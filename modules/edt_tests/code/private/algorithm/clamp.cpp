#include "edt/algorithm/clamp.hpp"

#include <cstdint>
#include <limits>

#include "gtest/gtest.h"

TEST(ClampTest, ClampToAnotherTypeBounds)
{
    // Signed to narrower signed
    EXPECT_EQ(edt::ClampToAnotherTypeBounds<std::int8_t>(std::int16_t{10}), 10);
    EXPECT_EQ(edt::ClampToAnotherTypeBounds<std::int8_t>(std::int16_t{130}), 127);
    EXPECT_EQ(edt::ClampToAnotherTypeBounds<std::int8_t>(std::int16_t{-130}), -128);

    // Signed to unsigned
    EXPECT_EQ(edt::ClampToAnotherTypeBounds<std::uint8_t>(std::int16_t{10}), 10);
    EXPECT_EQ(edt::ClampToAnotherTypeBounds<std::uint8_t>(std::int16_t{256}), 255);
    EXPECT_EQ(edt::ClampToAnotherTypeBounds<std::uint8_t>(std::int16_t{-10}), 0);

    // Unsigned to signed
    EXPECT_EQ(edt::ClampToAnotherTypeBounds<std::int8_t>(std::uint16_t{10}), 10);
    EXPECT_EQ(edt::ClampToAnotherTypeBounds<std::int8_t>(std::uint16_t{128}), 127);
}

// The bound does not fit in the source type. The old implementation converted
// Limits::max() into the source type first, which wrapped to -1 and clamped
// every input down to zero.
TEST(ClampTest, BoundNotRepresentableInSourceType)
{
    EXPECT_EQ(edt::ClampToAnotherTypeBounds<std::uint64_t>(std::int32_t{5}), 5);
    EXPECT_EQ(edt::ClampToAnotherTypeBounds<std::uint64_t>(std::int32_t{-7}), 0);
    EXPECT_EQ(edt::ClampToAnotherTypeBounds<std::int64_t>(std::uint32_t{4000000000U}), 4000000000LL);

    // Nothing to do when every value of From already fits in Bounds.
    EXPECT_EQ(edt::ClampToAnotherTypeBounds<std::int64_t>(std::int8_t{-128}), -128);
}

// Each bound is only checked when the source type can actually leave the range,
// so these three cases compile to different code.
TEST(ClampTest, OnlyReachableBoundsAreChecked)
{
    // Neither bound reachable: every int8_t is a valid int64_t.
    static_assert(edt::ClampToAnotherTypeBounds<std::int64_t>(std::int8_t{127}) == 127);
    static_assert(edt::ClampToAnotherTypeBounds<std::int64_t>(std::int8_t{-128}) == -128);

    // Only the lower bound reachable: an int32_t can go below 0 but never above
    // uint64_t's maximum.
    static_assert(edt::ClampToAnotherTypeBounds<std::uint64_t>(std::int32_t{-1}) == 0);
    static_assert(
        edt::ClampToAnotherTypeBounds<std::uint64_t>(std::int32_t{std::numeric_limits<std::int32_t>::max()}) ==
        std::numeric_limits<std::int32_t>::max());

    // Only the upper bound reachable: a uint16_t is never negative.
    static_assert(edt::ClampToAnotherTypeBounds<std::int8_t>(std::uint16_t{0}) == 0);
    static_assert(edt::ClampToAnotherTypeBounds<std::int8_t>(std::uint16_t{500}) == 127);
}

TEST(ClampTest, IsConstexpr)
{
    static_assert(edt::ClampToAnotherTypeBounds<std::int8_t>(std::int16_t{130}) == 127);
    static_assert(edt::ClampToAnotherTypeBounds<std::uint64_t>(std::int32_t{-7}) == 0);
}
