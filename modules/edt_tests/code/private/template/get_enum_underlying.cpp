#include "edt/template/get_enum_underlying.hpp"

#include <cstdint>
#include <type_traits>

#include "gtest/gtest.h"

namespace
{
enum class ScopedByte : uint8_t
{
    kZero = 0,
    kMax = 255,
};

enum class ScopedSigned : int16_t
{
    kNegative = -3,
    kLarge = 30000,
};

enum Unscoped
{
    kSmall = 7,
    kLarge = 0x7FFFFFFF,
};
}  // namespace

static_assert(std::is_same_v<decltype(edt::GetEnumUnderlying(ScopedByte::kZero)), uint8_t>);
static_assert(std::is_same_v<decltype(edt::GetEnumUnderlying(ScopedSigned::kNegative)), int16_t>);
static_assert(std::is_same_v<decltype(edt::GetEnumUnderlying(kSmall)), std::underlying_type_t<Unscoped>>);
static_assert(edt::GetEnumUnderlying(ScopedByte::kMax) == 255);
static_assert(edt::GetEnumUnderlying(ScopedSigned::kNegative) == -3);
static_assert(edt::GetEnumUnderlying(kSmall) == 7);

TEST(GetEnumUnderlyingTest, ReturnsTheUnderlyingValue)
{
    EXPECT_EQ(edt::GetEnumUnderlying(ScopedByte::kMax), 255);
    EXPECT_EQ(edt::GetEnumUnderlying(ScopedSigned::kLarge), 30000);
    EXPECT_EQ(edt::GetEnumUnderlying(kLarge), 0x7FFFFFFF);
}
