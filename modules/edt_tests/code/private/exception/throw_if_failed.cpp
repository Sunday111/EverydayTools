#include "edt/exception/throw_if_failed.hpp"

#include <stdexcept>
#include <string>

#include "gtest/gtest.h"

TEST(ThrowIfFailedTest, DoesNothingWhenConditionHolds)
{
    EXPECT_NO_THROW(edt::ThrowIfFailed(true, "never seen"));
    EXPECT_NO_THROW(edt::ThrowIfFailed(true, "never seen {}", 42));
}

TEST(ThrowIfFailedTest, ThrowsWithPlainMessage)
{
    try
    {
        edt::ThrowIfFailed(false, "plain message");
        FAIL() << "expected a throw";
    }
    catch (const std::runtime_error& e)
    {
        EXPECT_EQ(std::string(e.what()), "plain message");
    }
}

TEST(ThrowIfFailedTest, ThrowsWithFormattedMessage)
{
    try
    {
        edt::ThrowIfFailed(false, "index {} exceeds {}", 7, 3);
        FAIL() << "expected a throw";
    }
    catch (const std::runtime_error& e)
    {
        EXPECT_EQ(std::string(e.what()), "index 7 exceeds 3");
    }
}

TEST(ThrowIfFailedTest, HonoursTheExceptionType)
{
    EXPECT_THROW(edt::ThrowIfFailed<std::logic_error>(false, "nope"), std::logic_error);
}

// A message with no arguments takes the non-format overload, so braces in it
// are literal rather than being parsed as replacement fields.
TEST(ThrowIfFailedTest, PlainMessageKeepsBracesLiteral)
{
    try
    {
        edt::ThrowIfFailed(false, "a {braced} word");
        FAIL() << "expected a throw";
    }
    catch (const std::runtime_error& e)
    {
        EXPECT_EQ(std::string(e.what()), "a {braced} word");
    }
}
