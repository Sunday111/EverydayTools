#include "edt/algorithm/compare.hpp"

#include "gtest/gtest.h"

TEST(CompareTest, AlmostEqual)
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
