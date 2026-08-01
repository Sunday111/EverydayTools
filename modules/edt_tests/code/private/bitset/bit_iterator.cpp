#include "edt/bitset/bit_iterator.hpp"

#include <cstdint>
#include <limits>
#include <vector>

#include "gtest/gtest.h"

namespace
{
template <typename T>
std::vector<size_t> CollectSetBits(T word)
{
    std::vector<size_t> indices;
    edt::BitIterator iterator(word);
    while (const auto index = iterator.Next()) indices.push_back(*index);
    return indices;
}
}  // namespace

TEST(BitIteratorTest, EmptyBitsetYieldsNothing)  // NOLINT
{
    uint64_t word = 0;
    EXPECT_TRUE(CollectSetBits(word).empty());
}

// Repeated calls past the end keep returning nullopt rather than wrapping around.
TEST(BitIteratorTest, StaysExhausted)  // NOLINT
{
    uint64_t word = 0b101;
    edt::BitIterator iterator(word);
    EXPECT_EQ(iterator.Next(), std::optional<size_t>{0});
    EXPECT_EQ(iterator.Next(), std::optional<size_t>{2});
    for (int i = 0; i != 4; ++i) EXPECT_EQ(iterator.Next(), std::nullopt) << "call " << i;
}

// The lowest and highest bit of the word are the two that off-by-one errors hit.
TEST(BitIteratorTest, FindsBoundaryBits)  // NOLINT
{
    uint64_t lowest = 1;
    EXPECT_EQ(CollectSetBits(lowest), std::vector<size_t>{0});

    uint64_t highest = uint64_t{1} << 63;
    EXPECT_EQ(CollectSetBits(highest), std::vector<size_t>{63});

    uint64_t both = 1 | (uint64_t{1} << 63);
    EXPECT_EQ(CollectSetBits(both), (std::vector<size_t>{0, 63}));
}

TEST(BitIteratorTest, FindsEverySingleBitPosition)  // NOLINT
{
    for (size_t bit = 0; bit != 64; ++bit)
    {
        uint64_t word = uint64_t{1} << bit;
        EXPECT_EQ(CollectSetBits(word), std::vector<size_t>{bit}) << "bit = " << bit;
    }
}

TEST(BitIteratorTest, FullWordYieldsEveryIndexInOrder)  // NOLINT
{
    uint64_t word = std::numeric_limits<uint64_t>::max();
    const auto indices = CollectSetBits(word);

    ASSERT_EQ(indices.size(), 64U);
    for (size_t i = 0; i != 64; ++i) EXPECT_EQ(indices[i], i);
}

// Narrower words must stop at their own width, not at 64.
TEST(BitIteratorTest, RespectsTheWordWidth)  // NOLINT
{
    uint8_t byte = std::numeric_limits<uint8_t>::max();
    const auto indices = CollectSetBits(byte);
    ASSERT_EQ(indices.size(), 8U);
    EXPECT_EQ(indices.back(), 7U);

    uint16_t half = std::numeric_limits<uint16_t>::max();
    EXPECT_EQ(CollectSetBits(half).size(), 16U);

    uint32_t word = std::numeric_limits<uint32_t>::max();
    EXPECT_EQ(CollectSetBits(word).size(), 32U);
}

// The iterator views the word, so edits made through the reference are observed.
TEST(BitIteratorTest, ReadsThroughTheReference)  // NOLINT
{
    uint32_t word = 0b1;
    edt::BitIterator iterator(word);
    EXPECT_EQ(iterator.Next(), std::optional<size_t>{0});

    word |= 0b1000;
    ASSERT_EQ(word, 0b1001U);
    EXPECT_EQ(iterator.Next(), std::optional<size_t>{3});
}
