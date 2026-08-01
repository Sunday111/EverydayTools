#include "edt/bitset/fixed_bitset.hpp"

#include "gtest/gtest.h"

namespace
{
// Widths that are not multiples of the part width are the interesting ones: the
// last word is only partly in use, so Size() and the storage disagree.
template <size_t bits_count>
void CheckEveryBitIsIndependent()
{
    for (size_t index = 0; index != bits_count; ++index)
    {
        edt::FixedBitset<bits_count> bitset;
        bitset.Set(index, true);

        for (size_t i = 0; i != bits_count; ++i)
        {
            EXPECT_EQ(bitset.Get(i), i == index)
                << "bits_count = " << bits_count << ", set " << index << ", read " << i;
        }

        bitset.Set(index, false);
        for (size_t i = 0; i != bits_count; ++i)
        {
            EXPECT_FALSE(bitset.Get(i)) << "bits_count = " << bits_count << ", cleared " << index << ", read " << i;
        }
    }
}

template <size_t bits_count>
void CheckFillAndRangeCoverEverything()
{
    {
        edt::FixedBitset<bits_count> bitset;
        bitset.Fill(true);
        for (size_t i = 0; i != bits_count; ++i) EXPECT_TRUE(bitset.Get(i)) << "bits_count = " << bits_count;

        bitset.Fill(false);
        for (size_t i = 0; i != bits_count; ++i) EXPECT_FALSE(bitset.Get(i)) << "bits_count = " << bits_count;
    }

    // An empty range must change nothing, at either end of the bitset.
    {
        edt::FixedBitset<bits_count> bitset;
        bitset.Fill(true);
        bitset.SetRange(0, 0, false);
        bitset.SetRange(bits_count, bits_count, false);
        for (size_t i = 0; i != bits_count; ++i) EXPECT_TRUE(bitset.Get(i)) << "bits_count = " << bits_count;
    }

    // Every sub-range of the first few and last few bits.
    {
        for (size_t begin = 0; begin != bits_count; ++begin)
        {
            for (size_t end = begin; end <= bits_count; ++end)
            {
                edt::FixedBitset<bits_count> bitset;
                bitset.SetRange(begin, end, true);
                for (size_t i = 0; i != bits_count; ++i)
                {
                    EXPECT_EQ(bitset.Get(i), i >= begin && i < end)
                        << "bits_count = " << bits_count << ", [" << begin << ", " << end << "), i = " << i;
                }
            }
        }
    }
}
}  // namespace

// Widths straddling every part-size boundary, including one that is not a multiple
// of any of them.
static_assert(edt::FixedBitset<1>::Size() == 1);
static_assert(edt::FixedBitset<65>::Size() == 65);
static_assert(edt::FixedBitset<1>::PartsCount() == 1);
static_assert(edt::FixedBitset<65>::PartsCount() == 9);  // 9 one-byte parts

TEST(FixedBitsetTest, StartsAllZero)  // NOLINT
{
    const edt::FixedBitset<70> bitset;
    for (size_t i = 0; i != 70; ++i) EXPECT_FALSE(bitset.Get(i)) << "i = " << i;
}

TEST(FixedBitsetTest, EveryBitIsIndependent)  // NOLINT
{
    CheckEveryBitIsIndependent<1>();
    CheckEveryBitIsIndependent<7>();
    CheckEveryBitIsIndependent<8>();
    CheckEveryBitIsIndependent<9>();
    CheckEveryBitIsIndependent<63>();
    CheckEveryBitIsIndependent<64>();
    CheckEveryBitIsIndependent<65>();
    CheckEveryBitIsIndependent<129>();
}

TEST(FixedBitsetTest, FillAndSetRange)  // NOLINT
{
    CheckFillAndRangeCoverEverything<1>();
    CheckFillAndRangeCoverEverything<7>();
    CheckFillAndRangeCoverEverything<8>();
    CheckFillAndRangeCoverEverything<9>();
    CheckFillAndRangeCoverEverything<17>();
    CheckFillAndRangeCoverEverything<64>();
    CheckFillAndRangeCoverEverything<65>();
}

// A single-bit bitset is the smallest thing that can go wrong in the part
// arithmetic, and it still occupies a whole byte.
TEST(FixedBitsetTest, SingleBit)  // NOLINT
{
    static_assert(sizeof(edt::FixedBitset<1>) == 1);

    edt::FixedBitset<1> bitset;
    EXPECT_FALSE(bitset.Get(0));
    bitset.Set(0, true);
    EXPECT_TRUE(bitset.Get(0));
    bitset.Fill(false);
    EXPECT_FALSE(bitset.Get(0));
}

// Usable at compile time.
TEST(FixedBitsetTest, IsConstexpr)  // NOLINT
{
    constexpr bool kResult = []
    {
        edt::FixedBitset<65> bitset;
        bitset.Set(64, true);
        bitset.SetRange(0, 3, true);
        return bitset.Get(64) && bitset.Get(0) && bitset.Get(2) && !bitset.Get(3) && !bitset.Get(63);
    }();
    static_assert(kResult);
    EXPECT_TRUE(kResult);
}
