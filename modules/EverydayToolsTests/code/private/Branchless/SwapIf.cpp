#include "EverydayTools/Branchless/SwapIf.hpp"

#include <cstdint>
#include <limits>

#include "gtest/gtest.h"

using edt::SwapIf;

namespace
{
constexpr int SwapIfEncode(const bool condition)
{
    int a = 1;
    int b = 2;
    SwapIf(condition, a, b);
    return a * 10 + b;
}
}  // namespace

// Usable in constant expressions.
static_assert(SwapIfEncode(true) == 21);
static_assert(SwapIfEncode(false) == 12);

namespace
{
template <typename T>
void CheckSwapsOnlyWhenConditionSet()
{
    using L = std::numeric_limits<T>;
    const T values[] = {T{0}, T{1}, static_cast<T>(-1), L::min(), L::max()};
    for (const T x : values)
    {
        for (const T y : values)
        {
            T a = x;
            T b = y;

            SwapIf(false, a, b);
            EXPECT_EQ(a, x) << "no-swap changed a";
            EXPECT_EQ(b, y) << "no-swap changed b";

            SwapIf(true, a, b);
            EXPECT_EQ(a, y) << "swap failed for a";
            EXPECT_EQ(b, x) << "swap failed for b";
        }
    }
}
}  // namespace

TEST(SwapIfTest, SwapsOnlyWhenConditionSetForEveryIntegralType)  // NOLINT
{
    CheckSwapsOnlyWhenConditionSet<int8_t>();
    CheckSwapsOnlyWhenConditionSet<uint8_t>();
    CheckSwapsOnlyWhenConditionSet<int16_t>();
    CheckSwapsOnlyWhenConditionSet<uint16_t>();
    CheckSwapsOnlyWhenConditionSet<int32_t>();
    CheckSwapsOnlyWhenConditionSet<uint32_t>();
    CheckSwapsOnlyWhenConditionSet<int64_t>();
    CheckSwapsOnlyWhenConditionSet<uint64_t>();
}

TEST(SwapIfTest, SelfConsistentUnderRepeatedToggling)  // NOLINT
{
    int a = 12345;
    int b = -6789;
    const int a0 = a;
    const int b0 = b;
    // Swapping twice returns to the original arrangement.
    SwapIf(true, a, b);
    SwapIf(true, a, b);
    EXPECT_EQ(a, a0);
    EXPECT_EQ(b, b0);
}
