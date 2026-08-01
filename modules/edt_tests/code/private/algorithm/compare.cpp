#include "edt/algorithm/compare.hpp"

#include <array>
#include <cmath>
#include <limits>

#include "gtest/gtest.h"

TEST(CompareTest, AlmostEqual)  // NOLINT
{
    EXPECT_TRUE(edt::AlmostEqual(1.0f, 1.0f));
    EXPECT_TRUE(edt::AlmostEqual(1.0f, 1.0f + std::numeric_limits<float>::epsilon() / 2));
    EXPECT_FALSE(edt::AlmostEqual(1.0f, 1.1f));

    // Explicit tolerance, and symmetry in the sign of the difference.
    EXPECT_TRUE(edt::AlmostEqual(1.0, 1.05, 0.1));
    EXPECT_TRUE(edt::AlmostEqual(1.05, 1.0, 0.1));
    EXPECT_FALSE(edt::AlmostEqual(1.0, 1.5, 0.1));

    static_assert(edt::AlmostEqual(2.0, 2.0));
    static_assert(!edt::AlmostEqual(2.0, 3.0));
}

// The comparison is on the absolute difference, so it is exactly symmetric in its
// arguments - no ordering of a and b can disagree.
TEST(CompareTest, IsSymmetric)  // NOLINT
{
    constexpr std::array kValues{-1e9, -1.0, -0.5, 0.0, 0.5, 1.0, 1e9};
    for (const double a : kValues)
    {
        for (const double b : kValues)
        {
            EXPECT_EQ(edt::AlmostEqual(a, b, 0.75), edt::AlmostEqual(b, a, 0.75)) << "a = " << a << ", b = " << b;
        }
    }
}

// A zero tolerance degenerates to exact equality rather than accepting everything.
TEST(CompareTest, ZeroToleranceIsExactEquality)  // NOLINT
{
    EXPECT_TRUE(edt::AlmostEqual(1.0, 1.0, 0.0));
    EXPECT_FALSE(edt::AlmostEqual(1.0, std::nextafter(1.0, 2.0), 0.0));

    // The two zeros differ in sign but their difference is +0.0.
    EXPECT_TRUE(edt::AlmostEqual(0.0, -0.0, 0.0));
}

// NaN compares unequal to everything including itself: the difference is NaN, and
// every comparison against NaN is false.
TEST(CompareTest, NaNIsNeverAlmostEqual)  // NOLINT
{
    const double nan = std::numeric_limits<double>::quiet_NaN();
    const double inf = std::numeric_limits<double>::infinity();

    EXPECT_FALSE(edt::AlmostEqual(nan, nan));
    EXPECT_FALSE(edt::AlmostEqual(nan, 0.0));
    EXPECT_FALSE(edt::AlmostEqual(0.0, nan));
    EXPECT_FALSE(edt::AlmostEqual(nan, nan, inf));
}

// Worth pinning because it surprises: two identical infinities are NOT almost
// equal. inf - inf is NaN, and NaN fails the tolerance test. Callers feeding
// unbounded values through this need their own identity check first.
TEST(CompareTest, EqualInfinitiesAreNotAlmostEqual)  // NOLINT
{
    const double inf = std::numeric_limits<double>::infinity();

    EXPECT_FALSE(edt::AlmostEqual(inf, inf));
    EXPECT_FALSE(edt::AlmostEqual(-inf, -inf));

    // Opposite infinities differ by inf, which no finite tolerance covers, but an
    // infinite tolerance does accept them since inf <= inf.
    EXPECT_FALSE(edt::AlmostEqual(inf, -inf));
    EXPECT_TRUE(edt::AlmostEqual(inf, -inf, inf));
}

// The default tolerance is an absolute epsilon, not a relative one, so it means
// nothing once the values are large - neighbouring doubles are then further apart
// than epsilon - while values far below epsilon all collapse together.
TEST(CompareTest, DefaultToleranceIsAbsoluteNotRelative)  // NOLINT
{
    const double large = 1e16;
    EXPECT_FALSE(edt::AlmostEqual(large, std::nextafter(large, 1e17)));

    EXPECT_TRUE(edt::AlmostEqual(0.0, std::numeric_limits<double>::epsilon() / 2));
    EXPECT_TRUE(edt::AlmostEqual(0.0, std::numeric_limits<double>::denorm_min()));
    EXPECT_TRUE(edt::AlmostEqual(1e-20, -1e-20));
}

TEST(CompareTest, WorksForEveryFloatingPointType)  // NOLINT
{
    EXPECT_TRUE(edt::AlmostEqual(1.0f, 1.0f, 0.0f));
    EXPECT_TRUE(edt::AlmostEqual(1.0, 1.0, 0.0));
    EXPECT_TRUE(edt::AlmostEqual(1.0L, 1.0L, 0.0L));
    EXPECT_FALSE(edt::AlmostEqual(std::numeric_limits<float>::quiet_NaN(), 1.0f));
}
