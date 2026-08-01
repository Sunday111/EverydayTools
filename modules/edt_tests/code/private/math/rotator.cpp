#include "edt/math/rotator.hpp"

#include "gtest/gtest.h"

namespace
{
constexpr float kTolerance = 1e-5f;

void ExpectMatrixNear(const edt::Mat4f& actual, const edt::Mat4f& expected)
{
    for (size_t row = 0; row != 4; ++row)
    {
        for (size_t column = 0; column != 4; ++column)
        {
            EXPECT_NEAR(actual(row, column), expected(row, column), kTolerance)
                << "at [" << row << ", " << column << "]";
        }
    }
}

edt::Vec3f Rotate(const edt::Rotator& rotator, const edt::Vec3f& v)
{
    return edt::Math::TransformVector(rotator.ToMatrix(), v);
}
}  // namespace

TEST(RotatorTest, DefaultRotationIsIdentity)
{
    ExpectMatrixNear(edt::Rotator{}.ToMatrix(), edt::Mat4f::Identity());
}

TEST(RotatorTest, YawRotatesAroundZ)
{
    const edt::Rotator rotator{.yaw = 90.f};
    const auto rotated = Rotate(rotator, edt::Vec3f{1, 0, 0});

    EXPECT_NEAR(rotated.x(), 0.f, kTolerance);
    EXPECT_NEAR(rotated.y(), 1.f, kTolerance);
    EXPECT_NEAR(rotated.z(), 0.f, kTolerance);
}

TEST(RotatorTest, PitchRotatesAroundY)
{
    const edt::Rotator rotator{.pitch = 90.f};
    const auto rotated = Rotate(rotator, edt::Vec3f{1, 0, 0});

    EXPECT_NEAR(rotated.x(), 0.f, kTolerance);
    EXPECT_NEAR(rotated.y(), 0.f, kTolerance);
    EXPECT_NEAR(rotated.z(), -1.f, kTolerance);
}

TEST(RotatorTest, RollRotatesAroundX)
{
    const edt::Rotator rotator{.roll = 90.f};
    const auto rotated = Rotate(rotator, edt::Vec3f{0, 1, 0});

    EXPECT_NEAR(rotated.x(), 0.f, kTolerance);
    EXPECT_NEAR(rotated.y(), 0.f, kTolerance);
    EXPECT_NEAR(rotated.z(), 1.f, kTolerance);
}

TEST(RotatorTest, RotationPreservesLength)
{
    const edt::Rotator rotator{.yaw = 33.f, .pitch = -71.f, .roll = 128.f};
    const edt::Vec3f v{1, 2, 3};
    const auto rotated = Rotate(rotator, v);

    const float original = v.x() * v.x() + v.y() * v.y() + v.z() * v.z();
    const float result = rotated.x() * rotated.x() + rotated.y() * rotated.y() + rotated.z() * rotated.z();
    EXPECT_NEAR(original, result, 1e-4f);
}

TEST(RotatorTest, ArithmeticAppliesComponentWise)
{
    constexpr edt::Rotator a{.yaw = 10.f, .pitch = 20.f, .roll = 30.f};
    constexpr edt::Rotator b{.yaw = 1.f, .pitch = 2.f, .roll = 3.f};

    EXPECT_FLOAT_EQ((a + b).yaw, 11.f);
    EXPECT_FLOAT_EQ((a - b).pitch, 18.f);
    EXPECT_FLOAT_EQ((a * 2.f).roll, 60.f);
    EXPECT_FLOAT_EQ((a / 2.f).yaw, 5.f);
    EXPECT_FLOAT_EQ((a + 5.f).pitch, 25.f);
    EXPECT_FLOAT_EQ((a - 5.f).roll, 25.f);

    edt::Rotator mutated = a;
    mutated += b;
    EXPECT_FLOAT_EQ(mutated.roll, 33.f);
}
