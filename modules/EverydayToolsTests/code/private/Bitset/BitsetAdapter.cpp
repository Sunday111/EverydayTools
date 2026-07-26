#include "EverydayTools/Bitset/BitsetAdapter.hpp"

#include <cstdint>

#include "gtest/gtest.h"

namespace
{
template <typename T>
constexpr T Filled(const bool initial)
{
    return initial ? static_cast<T>(~T{0}) : T{0};
}

template <typename T>
T RefWithRange(const bool initial, const size_t begin, const size_t end, const bool value)
{
    T word = Filled<T>(initial);
    for (size_t i = begin; i != end; ++i)
    {
        const T bit = static_cast<T>(T{1} << i);
        word = value ? static_cast<T>(word | bit) : static_cast<T>(word & ~bit);
    }
    return word;
}

// The shift helpers are constexpr, so a whole set/get sequence is too.
constexpr uint16_t ConstexprSetRange()
{
    uint16_t word = 0;
    edt::BitsetAdapter adapter{word};
    adapter.SetRange(3, 11, true);
    return word;
}
}  // namespace

static_assert(ConstexprSetRange() == 0b0000'0111'1111'1000);

TEST(BitsetAdapterTest, SetRangeExhaustive)  // NOLINT
{
    using T = uint16_t;
    constexpr size_t kBits = 16;
    for (const bool initial : {false, true})
    {
        for (const bool value : {false, true})
        {
            for (size_t begin = 0; begin != kBits; ++begin)
            {
                for (size_t end = begin; end <= kBits; ++end)
                {
                    T word = Filled<T>(initial);
                    edt::BitsetAdapter adapter{word};
                    adapter.SetRange(begin, end, value);
                    EXPECT_EQ(word, RefWithRange<T>(initial, begin, end, value))
                        << "initial=" << initial << " value=" << value << " [" << begin << "," << end << ")";
                }
            }
        }
    }
}

TEST(BitsetAdapterTest, SetFirstN)  // NOLINT
{
    using T = uint16_t;
    constexpr size_t kBits = 16;
    for (const bool initial : {false, true})
    {
        for (const bool value : {false, true})
        {
            for (size_t count = 0; count <= kBits; ++count)
            {
                T word = Filled<T>(initial);
                edt::BitsetAdapter adapter{word};
                adapter.SetFirstN(count, value);
                EXPECT_EQ(word, RefWithRange<T>(initial, 0, count, value))
                    << "count=" << count << " value=" << value << " initial=" << initial;
            }
        }
    }
}

TEST(BitsetAdapterTest, SetLastN)  // NOLINT
{
    using T = uint16_t;
    constexpr size_t kBits = 16;
    for (const bool initial : {false, true})
    {
        for (const bool value : {false, true})
        {
            for (size_t count = 0; count <= kBits; ++count)
            {
                T word = Filled<T>(initial);
                edt::BitsetAdapter adapter{word};
                adapter.SetLastN(count, value);
                EXPECT_EQ(word, RefWithRange<T>(initial, kBits - count, kBits, value))
                    << "count=" << count << " value=" << value << " initial=" << initial;
            }
        }
    }
}

TEST(BitsetAdapterTest, Fill)  // NOLINT
{
    uint32_t word = 0xABCDEF01;
    edt::BitsetAdapter adapter{word};
    adapter.Fill(true);
    EXPECT_EQ(word, 0xFFFFFFFFu);
    adapter.Fill(false);
    EXPECT_EQ(word, 0u);
}

TEST(BitsetAdapterTest, SetMaskedMatchesReference)  // NOLINT
{
    using T = uint32_t;
    const T masks[] = {0u, ~0u, 0xF0F0F0F0u, 0x1u, 0x80000000u, 0xDEADBEEFu};
    const T inits[] = {0u, ~0u, 0x12345678u};
    for (const T init : inits)
    {
        for (const T mask : masks)
        {
            for (const bool value : {false, true})
            {
                T word = init;
                edt::BitsetAdapter adapter{word};
                adapter.SetMasked(mask, value);
                const T expected = value ? static_cast<T>(init | mask) : static_cast<T>(init & ~mask);
                EXPECT_EQ(word, expected) << "init=" << init << " mask=" << mask << " value=" << value;
            }
        }
    }
}
