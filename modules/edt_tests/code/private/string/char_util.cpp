#include "edt/string/char_util.hpp"

#include <limits>

#include "gtest/gtest.h"

using edt::ToLower;
using edt::ToUpper;

// Every distinct value a char can hold, whatever its signedness.
constexpr int kByteValueCount = 1 << std::numeric_limits<unsigned char>::digits;

// Usable in constant expressions - the reason these exist rather than <cctype>.
static_assert(ToLower('A') == 'a');
static_assert(ToLower('Z') == 'z');
static_assert(ToUpper('a') == 'A');
static_assert(ToUpper('z') == 'Z');

// The characters adjacent to the letter ranges are left alone: '@' and '[' bracket
// 'A'-'Z', '`' and '{' bracket 'a'-'z'.
static_assert(ToLower('@') == '@');
static_assert(ToLower('[') == '[');
static_assert(ToUpper('`') == '`');
static_assert(ToUpper('{') == '{');

TEST(CharUtilTest, ConvertsOnlyTheOppositeCase)  // NOLINT
{
    for (char c = 'A'; c <= 'Z'; ++c)
    {
        EXPECT_EQ(ToLower(c), 'a' + (c - 'A'));
        EXPECT_EQ(ToUpper(c), c);
    }

    for (char c = 'a'; c <= 'z'; ++c)
    {
        EXPECT_EQ(ToUpper(c), 'A' + (c - 'a'));
        EXPECT_EQ(ToLower(c), c);
    }
}

// Every byte outside the two letter ranges passes through unchanged, including the
// ones above 0x7F that would be undefined behaviour to hand to std::tolower.
TEST(CharUtilTest, LeavesNonLettersUntouched)  // NOLINT
{
    for (int value = 0; value != kByteValueCount; ++value)
    {
        const auto c = static_cast<char>(value);
        const bool is_letter = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
        if (is_letter) continue;
        EXPECT_EQ(ToLower(c), c) << "value = " << value;
        EXPECT_EQ(ToUpper(c), c) << "value = " << value;
    }
}

TEST(CharUtilTest, RoundTripsThroughBothDirections)  // NOLINT
{
    for (int value = 0; value != kByteValueCount; ++value)
    {
        const auto c = static_cast<char>(value);
        EXPECT_EQ(ToLower(ToUpper(c)), ToLower(c)) << "value = " << value;
        EXPECT_EQ(ToUpper(ToLower(c)), ToUpper(c)) << "value = " << value;
        EXPECT_EQ(ToLower(ToLower(c)), ToLower(c)) << "value = " << value;
        EXPECT_EQ(ToUpper(ToUpper(c)), ToUpper(c)) << "value = " << value;
    }
}
