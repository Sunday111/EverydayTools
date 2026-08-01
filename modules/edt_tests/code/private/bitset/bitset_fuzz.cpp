#include <array>
#include <cstdint>
#include <random>
#include <vector>

#include "edt/bitset/bitset_adapter.hpp"
#include "edt/bitset/dynamic_bitset.hpp"
#include "gtest/gtest.h"

namespace
{
// std::vector<bool> is the model: every operation is applied to both, and the two
// must agree bit for bit afterwards. The point is the interaction between
// operations - partial words left behind by one call being read or overwritten by
// the next - which fixed cases do not reach.
class ReferenceBitset
{
public:
    explicit ReferenceBitset(size_t size) : bits_(size, false) {}

    void Set(size_t index, bool value) { bits_[index] = value; }

    void SetRange(size_t begin, size_t end, bool value)
    {
        for (size_t i = begin; i < end; ++i) bits_[i] = value;
    }

    [[nodiscard]] bool Get(size_t index) const { return bits_[index]; }
    [[nodiscard]] size_t Size() const { return bits_.size(); }

    [[nodiscard]] std::vector<size_t> SetBits() const
    {
        std::vector<size_t> indices;
        for (size_t i = 0; i != bits_.size(); ++i)
        {
            if (bits_[i]) indices.push_back(i);
        }
        return indices;
    }

private:
    std::vector<bool> bits_;
};

// One round of random operations against one adapter shape.
template <typename Part, size_t kPartCount>
void FuzzAdapter(std::mt19937& generator, size_t operations)
{
    constexpr size_t kSize = kPartCount * sizeof(Part) * 8;

    std::array<Part, kPartCount> storage{};
    const edt::BitsetAdapter adapter{std::span{storage}};
    ReferenceBitset reference(kSize);

    ASSERT_EQ(adapter.Size(), kSize);

    std::uniform_int_distribution<size_t> index(0, kSize - 1);
    std::uniform_int_distribution<size_t> bound(0, kSize);
    std::uniform_int_distribution<int> which(0, 5);
    std::bernoulli_distribution flag(0.5);

    for (size_t step = 0; step != operations; ++step)
    {
        const bool value = flag(generator);
        switch (which(generator))
        {
        case 0:
        {
            const size_t i = index(generator);
            adapter.Set(i, value);
            reference.Set(i, value);
            break;
        }
        case 1:
        {
            // Deliberately unordered: begin > end must be a no-op, not a wild write.
            const size_t a = bound(generator);
            const size_t b = bound(generator);
            adapter.SetRange(a, b, value);
            reference.SetRange(a, b, value);
            break;
        }
        case 2:
        {
            const size_t begin = index(generator);
            const size_t count = bound(generator) % (kSize - begin + 1);
            adapter.SetN(begin, count, value);
            reference.SetRange(begin, begin + count, value);
            break;
        }
        case 3:
        {
            const size_t count = bound(generator);
            adapter.SetFirstN(count, value);
            reference.SetRange(0, count, value);
            break;
        }
        case 4:
        {
            const size_t count = bound(generator);
            adapter.SetLastN(count, value);
            reference.SetRange(kSize - count, kSize, value);
            break;
        }
        default:
            adapter.Fill(value);
            reference.SetRange(0, kSize, value);
            break;
        }

        for (size_t i = 0; i != kSize; ++i)
        {
            ASSERT_EQ(adapter.Get(i), reference.Get(i))
                << "step = " << step << ", bit = " << i << ", part size = " << sizeof(Part)
                << ", parts = " << kPartCount;
        }

        // Iteration must report exactly the set bits, in ascending order.
        std::vector<size_t> visited;
        adapter.ForEachBit([&](size_t bit) { visited.push_back(bit); });
        ASSERT_EQ(visited, reference.SetBits()) << "step = " << step << ", part size = " << sizeof(Part);
    }
}
}  // namespace

// Every part width, and both a single word and several - the multi-word paths
// (leading partial word, whole middle words, trailing partial word) only run when
// a range actually spans words.
TEST(BitsetFuzzTest, AdapterMatchesTheModel)  // NOLINT
{
    constexpr size_t kOperations = 2500;
    std::mt19937 generator(20260731);

    FuzzAdapter<std::uint8_t, 1>(generator, kOperations);
    FuzzAdapter<std::uint8_t, 7>(generator, kOperations);
    FuzzAdapter<std::uint16_t, 1>(generator, kOperations);
    FuzzAdapter<std::uint16_t, 3>(generator, kOperations);
    FuzzAdapter<std::uint32_t, 1>(generator, kOperations);
    FuzzAdapter<std::uint32_t, 2>(generator, kOperations);
    FuzzAdapter<std::uint64_t, 1>(generator, kOperations);
    FuzzAdapter<std::uint64_t, 3>(generator, kOperations);
}

// ForEachBitWithReturn stops at the first callback returning false, and reports
// having stopped.
TEST(BitsetFuzzTest, EarlyExitStopsAtTheRequestedBit)  // NOLINT
{
    std::mt19937 generator(20260731);
    std::uniform_int_distribution<std::uint64_t> value;

    for (size_t attempt = 0; attempt != 3000; ++attempt)
    {
        std::array<std::uint64_t, 2> storage{value(generator), value(generator)};
        const edt::BitsetAdapter adapter{std::span{storage}};

        std::vector<size_t> all;
        adapter.ForEachBit([&](size_t bit) { all.push_back(bit); });
        if (all.empty()) continue;

        const size_t stop_after = generator() % all.size();
        std::vector<size_t> seen;
        const bool completed = adapter.ForEachBitWithReturn(
            [&](size_t bit)
            {
                seen.push_back(bit);
                return seen.size() <= stop_after;
            });

        EXPECT_FALSE(completed) << "attempt = " << attempt;
        ASSERT_EQ(seen.size(), stop_after + 1) << "attempt = " << attempt;
        for (size_t i = 0; i != seen.size(); ++i) EXPECT_EQ(seen[i], all[i]);
    }
}

// The dynamic bitset, resized between rounds so the model has to survive growth
// and shrinkage rather than a fixed layout.
TEST(BitsetFuzzTest, DynamicBitsetMatchesTheModel)  // NOLINT
{
    std::mt19937 generator(20260731);
    std::uniform_int_distribution<size_t> size_of(1, 200);
    std::bernoulli_distribution flag(0.5);

    edt::DynamicBitset bitset;
    std::vector<bool> reference;

    for (size_t round = 0; round != 1200; ++round)
    {
        const size_t new_size = size_of(generator);
        const bool fill = flag(generator);

        bitset.Resize(new_size, fill);
        reference.resize(new_size, fill);
        ASSERT_EQ(bitset.Size(), reference.size()) << "round = " << round;

        std::uniform_int_distribution<size_t> index(0, new_size - 1);
        for (size_t step = 0; step != 8; ++step)
        {
            const bool value = flag(generator);
            if (flag(generator))
            {
                const size_t i = index(generator);
                bitset.Set(i, value);
                reference[i] = value;
            }
            else
            {
                size_t begin = index(generator);
                size_t end = index(generator);
                if (begin > end) std::swap(begin, end);
                bitset.SetRange(begin, end, value);
                for (size_t i = begin; i < end; ++i) reference[i] = value;
            }
        }

        for (size_t i = 0; i != new_size; ++i)
        {
            ASSERT_EQ(bitset.Get(i), reference[i]) << "round = " << round << ", bit = " << i;
        }
    }
}
