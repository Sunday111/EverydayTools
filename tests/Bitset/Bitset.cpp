#include <bitset>
#include <random>
#include <vector>

#include "EverydayTools/Bitset/DynamicBitset.hpp"
#include "EverydayTools/Bitset/FixedBitset.hpp"
#include "gtest/gtest.h"

using namespace edt;  // NOLINT

static_assert(sizeof(FixedBitset<7>) == 1);
static_assert(sizeof(FixedBitset<8>) == 1);
static_assert(sizeof(FixedBitset<9>) == 2);
static_assert(sizeof(FixedBitset<16>) == 2);
static_assert(sizeof(FixedBitset<32>) == 4);
static_assert(sizeof(FixedBitset<64>) == 8);
static_assert(sizeof(FixedBitset<65>) == 9);

static_assert(std::is_same_v<typename FixedBitset<7>::Part, uint8_t>);
static_assert(std::is_same_v<typename FixedBitset<8>::Part, uint8_t>);
static_assert(std::is_same_v<typename FixedBitset<9>::Part, uint16_t>);
static_assert(std::is_same_v<typename FixedBitset<16>::Part, uint16_t>);
static_assert(std::is_same_v<typename FixedBitset<32>::Part, uint32_t>);
static_assert(std::is_same_v<typename FixedBitset<64>::Part, uint64_t>);
static_assert(std::is_same_v<typename FixedBitset<65>::Part, uint8_t>);

TEST(FixedBitsetTest, SetGet)  // NOLINT
{
    constexpr unsigned kSeed = 0;
    constexpr size_t kBitsCount = 37;
    constexpr size_t kFlipsCount = 1'000'000;
    FixedBitset<kBitsCount> edt_bitset;
    std::bitset<kBitsCount> std_bitset;
    std::mt19937 generator(kSeed);  // NOLINT

    for (size_t index = 0; index != std_bitset.size(); ++index)
    {
        ASSERT_EQ(std_bitset[index], edt_bitset.Get(index)) << "bit_index = " << index;
    }

    std::uniform_int_distribution<size_t> index_distributor(0, kBitsCount - 1);
    for (size_t flip_index = 0; flip_index != kFlipsCount; ++flip_index)
    {
        const size_t index = index_distributor(generator);
        const bool prev_value = std_bitset[index];
        const bool new_value = !prev_value;

        std_bitset[index] = new_value;
        ASSERT_EQ(std_bitset[index], new_value);

        ASSERT_EQ(edt_bitset.Get(index), prev_value);
        edt_bitset.Set(index, new_value);
        ASSERT_EQ(edt_bitset.Get(index), new_value);

        for (size_t index = 0; index != std_bitset.size(); ++index)
        {
            ASSERT_EQ(std_bitset[index], edt_bitset.Get(index))
                << "bit_index = " << index << ", flip index = " << index;
        }
    }
}

TEST(DynamicBitsetTest, ResizeSetGet)  // NOLINT
{
    DynamicBitset bitset;
    std::vector<bool> vector;

    constexpr unsigned kSeed = 0;
    constexpr size_t kResizesCount = 1'000;
    constexpr size_t kMinSize = 1;
    constexpr size_t kMaxSize = 1'000;
    constexpr size_t kMinFlipsCount = 50;
    constexpr size_t kMaxFlipsCount = 100;
    std::mt19937 generator(kSeed);  // NOLINT
    std::uniform_int_distribution<size_t> size_distributor(kMinSize, kMaxSize);
    std::uniform_int_distribution<size_t> flips_count_distributor(kMinFlipsCount, kMaxFlipsCount);

    for (size_t resize_index = 0; resize_index != kResizesCount; ++resize_index)
    {
        const size_t new_size = size_distributor(generator);

        vector.resize(new_size);
        ASSERT_EQ(vector.size(), new_size);

        bitset.Resize(new_size, false);
        ASSERT_EQ(bitset.Size(), vector.size());

        for (size_t index = 0; index != vector.size(); ++index)
        {
            ASSERT_EQ(vector[index], bitset.Get(index))
                << "resize_index = " << resize_index << ", bit_index = " << index;
        }

        std::uniform_int_distribution<size_t> index_distributor(0, new_size - 1);
        const size_t flips_count = flips_count_distributor(generator) % new_size;
        for (size_t flip_index = 0; flip_index != flips_count; ++flip_index)
        {
            const size_t index = index_distributor(generator);

            const bool prev_value = vector[index];
            const bool new_value = !prev_value;

            vector[index] = new_value;
            ASSERT_EQ(vector[index], new_value);

            ASSERT_EQ(bitset.Get(index), prev_value);
            bitset.Set(index, new_value);
            ASSERT_EQ(bitset.Get(index), new_value);

            for (size_t index = 0; index != vector.size(); ++index)
            {
                ASSERT_EQ(vector[index], bitset.Get(index))
                    << "resize_index = " << resize_index << ", bit_index = " << index << ", flip index = " << index;
            }
        }
    }
}

TEST(BitsetAdapterTest, ForEach)  // NOLINT
{
    uint64_t bitset = 0;
    constexpr unsigned kSeed = 0;
    std::mt19937 generator(kSeed);  // NOLINT
    std::uniform_int_distribution<uint64_t> value_distribution;

    edt::BitsetAdapter adapter(bitset);

    std::vector<size_t> expected;
    std::vector<size_t> actual;

    for (size_t i = 0; i != 1'000'000; ++i)
    {
        bitset = value_distribution(generator);
        expected.clear();
        actual.clear();

        for (size_t i = 0; i != adapter.kBitsCount; ++i)
        {
            if (adapter.Get(i))
            {
                expected.push_back(i);
            }
        }

        adapter.ForEachBit(
            [&](const size_t bit_index)
            {
                actual.push_back(bit_index);
            });

        ASSERT_EQ(expected, actual) << "i = " << i;
    }
}

TEST(BitIteratorTest, Test)  // NOLINT
{
    uint64_t bitset = 0;
    constexpr unsigned kSeed = 0;
    std::mt19937 generator(kSeed);  // NOLINT
    std::uniform_int_distribution<uint64_t> value_distribution;

    std::vector<size_t> expected;
    std::vector<size_t> actual;
    edt::BitsetAdapter adapter(bitset);

    for (size_t i = 0; i != 1'000'000; ++i)
    {
        bitset = value_distribution(generator);
        expected.clear();
        actual.clear();

        BitIterator iterator(bitset);
        while (auto opt = iterator.Next())
        {
            actual.push_back(*opt);
        }

        for (size_t i = 0; i != adapter.kBitsCount; ++i)
        {
            if (adapter.Get(i))
            {
                expected.push_back(i);
            }
        }

        ASSERT_EQ(expected, actual) << "i = " << i;
    }
}
