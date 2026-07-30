#include "edt/exception/checked_cast.hpp"

#include <cstdint>
#include <stdexcept>

#include "gtest/gtest.h"

TEST(CheckedCastTest, PassesValuesThatFit)
{
    EXPECT_EQ(edt::CheckedCast<std::int8_t>(std::int16_t{127}), 127);
    EXPECT_EQ(edt::CheckedCast<std::int8_t>(std::int16_t{-128}), -128);
    EXPECT_EQ(edt::CheckedCast<std::uint8_t>(std::int16_t{255}), 255);
    EXPECT_EQ(edt::CheckedCast<std::int64_t>(std::uint32_t{4000000000U}), 4000000000LL);
}

TEST(CheckedCastTest, ThrowsOnOverflow)
{
    EXPECT_THROW((void)edt::CheckedCast<std::int8_t>(std::int16_t{128}), std::runtime_error);
    EXPECT_THROW((void)edt::CheckedCast<std::int8_t>(std::int16_t{-129}), std::runtime_error);
    EXPECT_THROW((void)edt::CheckedCast<std::uint8_t>(std::int16_t{-1}), std::runtime_error);
    EXPECT_THROW((void)edt::CheckedCast<std::uint32_t>(std::int64_t{-1}), std::runtime_error);
}

// Mixed signedness where the bound cannot be represented in the source type.
// Comparing by hand converts the bound first and gets the wrong answer.
TEST(CheckedCastTest, ComparesAcrossSignednessCorrectly)
{
    EXPECT_EQ(edt::CheckedCast<std::uint64_t>(std::int32_t{5}), 5U);
    EXPECT_THROW((void)edt::CheckedCast<std::uint64_t>(std::int32_t{-1}), std::runtime_error);

    constexpr auto max_u64 = std::numeric_limits<std::uint64_t>::max();
    EXPECT_THROW((void)edt::CheckedCast<std::int64_t>(max_u64), std::runtime_error);
}

TEST(CheckedCastTest, IsConstexprOnTheSuccessPath)
{
    static_assert(edt::CheckedCast<std::int8_t>(std::int16_t{5}) == 5);
}
