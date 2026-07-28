#include "edt/branchless/int_if.hpp"

#include <array>
#include <cstdint>
#include <limits>

#include "gtest/gtest.h"

using edt::IntIf;

// Usable in constant expressions.
static_assert(IntIf(true, 5, 9) == 5);
static_assert(IntIf(false, 5, 9) == 9);
static_assert(IntIf(true, -1, 0) == -1);
static_assert(IntIf(false, -1, 0) == 0);
static_assert(IntIf(true, uint8_t{0xFF}, uint8_t{0x00}) == 0xFF);
static_assert(IntIf(false, uint64_t{1} << 63, uint64_t{7}) == 7);

namespace
{
template <typename T>
void CheckSelectsCorrectOperand()
{
    using L = std::numeric_limits<T>;
    const std::array<T, 6> values{T{0}, T{1}, static_cast<T>(-1), L::min(), L::max(), static_cast<T>(L::max() / 2)};
    for (const T a : values)
    {
        for (const T b : values)
        {
            EXPECT_EQ(IntIf(true, a, b), a) << "a=" << +a << " b=" << +b;
            EXPECT_EQ(IntIf(false, a, b), b) << "a=" << +a << " b=" << +b;
        }
    }
}
}  // namespace

TEST(IntIfTest, SelectsCorrectOperandForEveryIntegralType)  // NOLINT
{
    CheckSelectsCorrectOperand<int8_t>();
    CheckSelectsCorrectOperand<uint8_t>();
    CheckSelectsCorrectOperand<int16_t>();
    CheckSelectsCorrectOperand<uint16_t>();
    CheckSelectsCorrectOperand<int32_t>();
    CheckSelectsCorrectOperand<uint32_t>();
    CheckSelectsCorrectOperand<int64_t>();
    CheckSelectsCorrectOperand<uint64_t>();
}

TEST(IntIfTest, MatchesTernary)  // NOLINT
{
    for (int i = -2000; i <= 2000; ++i)
    {
        const int a = i * 3 + 1;
        const int b = i - 7;
        EXPECT_EQ(IntIf(true, a, b), true ? a : b);
        EXPECT_EQ(IntIf(false, a, b), false ? a : b);
    }
}

TEST(IntIfTest, SecondOperandTypeFollowsFirst)  // NOLINT
{
    // std::type_identity_t on the second parameter means the return type is
    // pinned by the first operand; a bare literal for the second still compiles.
    const uint16_t a = 0xBEEF;
    EXPECT_EQ(IntIf(true, a, 0), 0xBEEF);
    EXPECT_EQ(IntIf(false, a, 0), 0);
}
