#include "edt/bitset/dynamic_bitset.hpp"

#include <vector>

#include "gtest/gtest.h"

namespace
{
std::vector<bool> ReadAll(const edt::DynamicBitset& bitset)
{
    std::vector<bool> values;
    values.reserve(bitset.Size());
    for (size_t i = 0; i != bitset.Size(); ++i) values.push_back(bitset.Get(i));
    return values;
}
}  // namespace

TEST(DynamicBitsetTest, StartsEmpty)  // NOLINT
{
    const edt::DynamicBitset bitset;
    EXPECT_EQ(bitset.Size(), 0U);
}

// Resizing to zero is legal and leaves nothing readable behind.
TEST(DynamicBitsetTest, ResizeToZero)  // NOLINT
{
    edt::DynamicBitset bitset;
    bitset.Resize(10, true);
    EXPECT_EQ(bitset.Size(), 10U);

    bitset.Resize(0, false);
    EXPECT_EQ(bitset.Size(), 0U);

    bitset.Resize(3, true);
    EXPECT_EQ(ReadAll(bitset), (std::vector<bool>{true, true, true}));
}

// Growing fills the new tail with the requested value and leaves existing bits be.
TEST(DynamicBitsetTest, GrowingPreservesOldBitsAndFillsTheNewOnes)  // NOLINT
{
    for (const bool fill : {false, true})
    {
        edt::DynamicBitset bitset;
        bitset.Resize(4, !fill);
        bitset.Set(1, fill);

        bitset.Resize(8, fill);

        ASSERT_EQ(bitset.Size(), 8U);
        EXPECT_EQ(bitset.Get(0), !fill);
        EXPECT_EQ(bitset.Get(1), fill);
        EXPECT_EQ(bitset.Get(2), !fill);
        EXPECT_EQ(bitset.Get(3), !fill);
        for (size_t i = 4; i != 8; ++i) EXPECT_EQ(bitset.Get(i), fill) << "i = " << i;
    }
}

// Shrinking drops bits; growing again must produce the requested value rather than
// whatever was left in the underlying words.
TEST(DynamicBitsetTest, RegrownBitsAreNotStale)  // NOLINT
{
    edt::DynamicBitset bitset;
    bitset.Resize(200, true);
    ASSERT_TRUE(bitset.Get(199));

    bitset.Resize(5, false);
    ASSERT_EQ(bitset.Size(), 5U);

    bitset.Resize(200, false);
    ASSERT_EQ(bitset.Size(), 200U);
    for (size_t i = 5; i != 200; ++i) EXPECT_FALSE(bitset.Get(i)) << "i = " << i;

    // The bits that survived the shrink keep their old value.
    for (size_t i = 0; i != 5; ++i) EXPECT_TRUE(bitset.Get(i)) << "i = " << i;
}

// Growing within a single word, which takes a different path from adding words.
TEST(DynamicBitsetTest, GrowsWithinOneWord)  // NOLINT
{
    edt::DynamicBitset bitset;
    bitset.Resize(1, true);
    bitset.Resize(2, false);
    bitset.Resize(3, true);
    EXPECT_EQ(ReadAll(bitset), (std::vector<bool>{true, false, true}));
}

// Word boundaries are where an off-by-one in the part arithmetic shows up.
TEST(DynamicBitsetTest, HandlesWordBoundaries)  // NOLINT
{
    constexpr size_t kPartBits = edt::DynamicBitset::kPartSizeBits;
    for (const size_t size : {kPartBits - 1, kPartBits, kPartBits + 1, 2 * kPartBits, 2 * kPartBits + 1})
    {
        edt::DynamicBitset bitset;
        bitset.Resize(size, false);
        ASSERT_EQ(bitset.Size(), size);

        bitset.Set(0, true);
        bitset.Set(size - 1, true);

        EXPECT_TRUE(bitset.Get(0)) << "size = " << size;
        EXPECT_TRUE(bitset.Get(size - 1)) << "size = " << size;
        for (size_t i = 1; i + 1 < size; ++i) EXPECT_FALSE(bitset.Get(i)) << "size = " << size << ", i = " << i;
    }
}

TEST(DynamicBitsetTest, SetRangeSpansWords)  // NOLINT
{
    constexpr size_t kPartBits = edt::DynamicBitset::kPartSizeBits;
    edt::DynamicBitset bitset;
    bitset.Resize(3 * kPartBits, false);

    const size_t begin = kPartBits - 3;
    const size_t end = 2 * kPartBits + 3;
    bitset.SetRange(begin, end, true);

    for (size_t i = 0; i != bitset.Size(); ++i)
    {
        EXPECT_EQ(bitset.Get(i), i >= begin && i < end) << "i = " << i;
    }
}

// A range covering exactly one bit, and the smallest non-empty range at the very end.
TEST(DynamicBitsetTest, SetRangeOfOneBit)  // NOLINT
{
    edt::DynamicBitset bitset;
    bitset.Resize(70, false);

    bitset.SetRange(0, 1, true);
    bitset.SetRange(69, 70, true);

    EXPECT_TRUE(bitset.Get(0));
    EXPECT_TRUE(bitset.Get(69));
    for (size_t i = 1; i != 69; ++i) EXPECT_FALSE(bitset.Get(i)) << "i = " << i;
}

TEST(DynamicBitsetTest, ClearEmptiesIt)  // NOLINT
{
    edt::DynamicBitset bitset;
    bitset.Resize(100, true);
    bitset.Clear();
    EXPECT_EQ(bitset.Size(), 0U);

    // Reusable afterwards, with no bits carried over.
    bitset.Resize(100, false);
    ASSERT_EQ(bitset.Size(), 100U);
    for (size_t i = 0; i != 100; ++i) EXPECT_FALSE(bitset.Get(i)) << "i = " << i;
}
