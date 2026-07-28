#include "edt/bitset/bitset_utilities.hpp"

#include <cstdint>
#include <limits>

#include "gtest/gtest.h"

TEST(BitsetUtilitiesTest, MakeFlagByIndex)
{
    static_assert(edt::MakeFlagByIndex<std::uint32_t>(0) == 1U);
    static_assert(edt::MakeFlagByIndex<std::uint32_t>(3) == 8U);
    static_assert(edt::MakeFlagByIndex<std::uint32_t, 5>() == 32U);
}

// std::popcount, in place of a loop over every bit position.
TEST(BitsetUtilitiesTest, GetSetFlagsCount)
{
    static_assert(edt::GetSetFlagsCount(std::uint8_t{0}) == 0);
    static_assert(edt::GetSetFlagsCount(std::uint8_t{0b1011}) == 3);
    static_assert(edt::GetSetFlagsCount(std::numeric_limits<std::uint8_t>::max()) == 8);
    static_assert(edt::GetSetFlagsCount(std::numeric_limits<std::uint64_t>::max()) == 64);

    // Signed input: the sign bit counts like any other.
    static_assert(edt::GetSetFlagsCount(std::int8_t{-1}) == 8);
}

TEST(BitsetUtilitiesTest, BitCountToType)
{
    static_assert(std::is_same_v<edt::BitCountToTypeT<8>, std::uint8_t>);
    static_assert(std::is_same_v<edt::BitCountToTypeT<16>, std::uint16_t>);
    static_assert(std::is_same_v<edt::BitCountToTypeT<32>, std::uint32_t>);
    static_assert(std::is_same_v<edt::BitCountToTypeT<64>, std::uint64_t>);
}
