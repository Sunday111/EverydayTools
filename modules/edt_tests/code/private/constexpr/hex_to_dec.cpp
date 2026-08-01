#include "edt/constexpr/hex_to_dec.hpp"

#include <string_view>

#include "gtest/gtest.h"

// Usable in constant expressions - GUID parsing depends on it.
static_assert(edt::HexToDec('0') == 0);
static_assert(edt::HexToDec('9') == 9);
static_assert(edt::HexToDec('a') == 10);
static_assert(edt::HexToDec('f') == 15);
static_assert(edt::HexToDec('A') == 10);
static_assert(edt::HexToDec('F') == 15);

TEST(HexToDecTest, CoversTheWholeValidDomain)  // NOLINT
{
    constexpr std::string_view kDigits = "0123456789abcdef";
    constexpr std::string_view kUpper = "0123456789ABCDEF";

    for (size_t i = 0; i != kDigits.size(); ++i)
    {
        EXPECT_EQ(edt::HexToDec(kDigits[i]), i) << "symbol = " << kDigits[i];
        EXPECT_EQ(edt::HexToDec(kUpper[i]), i) << "symbol = " << kUpper[i];
    }
}

// Case is the only thing normalised: the two spellings of a digit agree exactly.
TEST(HexToDecTest, IsCaseInsensitive)  // NOLINT
{
    for (char c = 'a'; c <= 'f'; ++c)
    {
        EXPECT_EQ(edt::HexToDec(c), edt::HexToDec(edt::ToUpper(c))) << "symbol = " << c;
    }
}

// The boundaries of the two accepted runs, where an off-by-one in either branch
// of the comparison would show up first.
TEST(HexToDecTest, HandlesTheRangeBoundaries)  // NOLINT
{
    EXPECT_EQ(edt::HexToDec('0'), 0);
    EXPECT_EQ(edt::HexToDec('9'), 9);
    EXPECT_EQ(edt::HexToDec('a'), 10);
    EXPECT_EQ(edt::HexToDec('f'), 15);
    EXPECT_EQ(edt::HexToDec('A'), 10);
    EXPECT_EQ(edt::HexToDec('F'), 15);
}
