#include <concepts>
#include <cstdint>

#include "edt/float_aliases.hpp"
#include "edt/int_aliases.hpp"
#include "gtest/gtest.h"

namespace
{
using namespace edt::int_aliases;    // NOLINT(google-build-using-namespace): what the namespace is for
using namespace edt::float_aliases;  // NOLINT(google-build-using-namespace): what the namespace is for
}  // namespace

TEST(IntAliasesTest, WidthsAndSignedness)
{
    static_assert(std::same_as<u8, std::uint8_t> && sizeof(u8) == 1 && std::unsigned_integral<u8>);
    static_assert(std::same_as<u16, std::uint16_t> && sizeof(u16) == 2 && std::unsigned_integral<u16>);
    static_assert(std::same_as<u32, std::uint32_t> && sizeof(u32) == 4 && std::unsigned_integral<u32>);
    static_assert(std::same_as<u64, std::uint64_t> && sizeof(u64) == 8 && std::unsigned_integral<u64>);

    static_assert(std::same_as<i8, std::int8_t> && sizeof(i8) == 1 && std::signed_integral<i8>);
    static_assert(std::same_as<i16, std::int16_t> && sizeof(i16) == 2 && std::signed_integral<i16>);
    static_assert(std::same_as<i32, std::int32_t> && sizeof(i32) == 4 && std::signed_integral<i32>);
    static_assert(std::same_as<i64, std::int64_t> && sizeof(i64) == 8 && std::signed_integral<i64>);
}

TEST(FloatAliasesTest, WidthsAndIeeeLayout)
{
    static_assert(std::same_as<f32, float> && sizeof(f32) == 4);
    static_assert(std::same_as<f64, double> && sizeof(f64) == 8);

    static_assert(std::numeric_limits<f32>::is_iec559);
    static_assert(std::numeric_limits<f64>::is_iec559);
}

// The aliases exist to be pulled in wholesale; nothing may leak without that.
TEST(AliasesTest, NamespaceScopedUntilHoisted)
{
    static_assert(std::same_as<edt::int_aliases::u32, std::uint32_t>);
    static_assert(std::same_as<edt::float_aliases::f64, double>);
}
