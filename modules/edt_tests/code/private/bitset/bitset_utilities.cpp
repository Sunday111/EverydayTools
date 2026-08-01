#include "edt/bitset/bitset_utilities.hpp"

#include <cstdint>
#include <limits>

#include "gtest/gtest.h"

TEST(BitsetUtilitiesTest, MakeFlagByIndex)  // NOLINT
{
    static_assert(edt::MakeFlagByIndex<std::uint32_t>(0) == 1U);
    static_assert(edt::MakeFlagByIndex<std::uint32_t>(3) == 8U);
    static_assert(edt::MakeFlagByIndex<std::uint32_t, 5>() == 32U);
}

// The top bit of each width: the shift result has to survive conversion back to a
// type narrower than int, which is where an implicit promotion would lose it.
TEST(BitsetUtilitiesTest, MakeFlagByIndexAtTheTopBit)  // NOLINT
{
    static_assert(edt::MakeFlagByIndex<std::uint8_t>(7) == 0x80U);
    static_assert(edt::MakeFlagByIndex<std::uint16_t>(15) == 0x8000U);
    static_assert(edt::MakeFlagByIndex<std::uint32_t>(31) == 0x8000'0000U);
    static_assert(edt::MakeFlagByIndex<std::uint64_t>(63) == 0x8000'0000'0000'0000ULL);

    static_assert(edt::MakeFlagByIndex<std::uint8_t, 7>() == 0x80U);
    static_assert(edt::MakeFlagByIndex<std::uint64_t, 63>() == 0x8000'0000'0000'0000ULL);
}

// Shifting into the sign bit of a signed type is well defined since C++20 - it
// wraps to the minimum value rather than being undefined behaviour.
TEST(BitsetUtilitiesTest, MakeFlagByIndexIntoASignBit)  // NOLINT
{
    static_assert(edt::MakeFlagByIndex<std::int8_t>(7) == std::numeric_limits<std::int8_t>::min());
    static_assert(edt::MakeFlagByIndex<std::int32_t>(31) == std::numeric_limits<std::int32_t>::min());
    static_assert(edt::MakeFlagByIndex<std::int64_t>(63) == std::numeric_limits<std::int64_t>::min());
}

// Every index of a width produces a distinct single-bit value.
TEST(BitsetUtilitiesTest, MakeFlagByIndexCoversEveryBitExactlyOnce)  // NOLINT
{
    std::uint64_t combined = 0;
    for (std::size_t index = 0; index != 64; ++index)
    {
        const auto flag = edt::MakeFlagByIndex<std::uint64_t>(index);
        EXPECT_EQ(edt::GetSetFlagsCount(flag), 1U) << "index = " << index;
        EXPECT_EQ(combined & flag, 0U) << "index = " << index;
        combined |= flag;
    }
    EXPECT_EQ(combined, std::numeric_limits<std::uint64_t>::max());
}

// std::popcount, in place of a loop over every bit position.
TEST(BitsetUtilitiesTest, GetSetFlagsCount)  // NOLINT
{
    static_assert(edt::GetSetFlagsCount(std::uint8_t{0}) == 0);
    static_assert(edt::GetSetFlagsCount(std::uint8_t{0b1011}) == 3);
    static_assert(edt::GetSetFlagsCount(std::numeric_limits<std::uint8_t>::max()) == 8);
    static_assert(edt::GetSetFlagsCount(std::numeric_limits<std::uint64_t>::max()) == 64);

    // Signed input: the sign bit counts like any other.
    static_assert(edt::GetSetFlagsCount(std::int8_t{-1}) == 8);
}

// Signed values are counted through their unsigned representation, so only the
// width matters - not how the value would compare as a number.
TEST(BitsetUtilitiesTest, GetSetFlagsCountOfSignedExtremes)  // NOLINT
{
    static_assert(edt::GetSetFlagsCount(std::int8_t{0}) == 0);
    static_assert(edt::GetSetFlagsCount(std::numeric_limits<std::int8_t>::min()) == 1);
    static_assert(edt::GetSetFlagsCount(std::numeric_limits<std::int8_t>::max()) == 7);
    static_assert(edt::GetSetFlagsCount(std::numeric_limits<std::int64_t>::min()) == 1);
    static_assert(edt::GetSetFlagsCount(std::numeric_limits<std::int64_t>::max()) == 63);
    static_assert(edt::GetSetFlagsCount(std::int64_t{-1}) == 64);
}

TEST(BitsetUtilitiesTest, BitCountToType)  // NOLINT
{
    static_assert(std::is_same_v<edt::BitCountToTypeT<8>, std::uint8_t>);
    static_assert(std::is_same_v<edt::BitCountToTypeT<16>, std::uint16_t>);
    static_assert(std::is_same_v<edt::BitCountToTypeT<32>, std::uint32_t>);
    static_assert(std::is_same_v<edt::BitCountToTypeT<64>, std::uint64_t>);

    // The mapping agrees with the width it names.
    static_assert(edt::SizeInBits<edt::BitCountToTypeT<8>> == 8);
    static_assert(edt::SizeInBits<edt::BitCountToTypeT<64>> == 64);
}

// Folding a list of flags together, and testing membership of the result.
TEST(BitsetUtilitiesTest, FlagsToBitset)  // NOLINT
{
    constexpr auto kBitset = edt::FlagsToBitset(edt::ValuesList<std::uint8_t{0b0001}, std::uint8_t{0b0100}>{});
    static_assert(kBitset == 0b0101);
    static_assert(edt::GetSetFlagsCount(kBitset) == 2);

    static_assert(edt::FlagIsSet<std::uint8_t>(kBitset, 0b0001));
    static_assert(edt::FlagIsSet<std::uint8_t>(kBitset, 0b0100));
    static_assert(!edt::FlagIsSet<std::uint8_t>(kBitset, 0b0010));

    // A single-element list folds to that element; the value survives unchanged.
    static_assert(edt::FlagsToBitset(edt::ValuesList<std::uint32_t{0x8000'0000}>{}) == 0x8000'0000U);
}

// Zero is the identity: no flag is ever set in it, and nothing is set in a zero query.
TEST(BitsetUtilitiesTest, FlagIsSetDegenerateCases)  // NOLINT
{
    static_assert(!edt::FlagIsSet<std::uint8_t>(0, 0));
    static_assert(!edt::FlagIsSet<std::uint8_t>(0, 0b1));
    static_assert(!edt::FlagIsSet<std::uint8_t>(0b1, 0));
    static_assert(edt::FlagIsSet<std::uint8_t>(0xFF, 0x80));
    static_assert(edt::FlagIsSet<std::uint64_t>(std::numeric_limits<std::uint64_t>::max(), std::uint64_t{1} << 63));

    // Overlapping but not contained: any shared bit counts as set.
    static_assert(edt::FlagIsSet<std::uint8_t>(0b0110, 0b0011));
}
