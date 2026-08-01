#include "edt/math/transform.hpp"

#include "gtest/gtest.h"

namespace
{
constexpr float kTolerance = 1e-5f;

void ExpectVectorNear(const edt::Vec3f& actual, const edt::Vec3f& expected)
{
    EXPECT_NEAR(actual.x(), expected.x(), kTolerance);
    EXPECT_NEAR(actual.y(), expected.y(), kTolerance);
    EXPECT_NEAR(actual.z(), expected.z(), kTolerance);
}
}  // namespace

TEST(TransformTest, DefaultTransformIsIdentity)
{
    const edt::Transform transform;
    ExpectVectorNear(edt::Math::TransformPos(transform.Matrix(), edt::Vec3f{1, 2, 3}), edt::Vec3f{1, 2, 3});
}

TEST(TransformTest, TranslationMovesPositionsButNotVectors)
{
    const edt::Transform transform{.translation = {10, 20, 30}};

    ExpectVectorNear(edt::Math::TransformPos(transform.Matrix(), edt::Vec3f{1, 2, 3}), edt::Vec3f{11, 22, 33});
    ExpectVectorNear(edt::Math::TransformVector(transform.Matrix(), edt::Vec3f{1, 2, 3}), edt::Vec3f{1, 2, 3});
}

TEST(TransformTest, ScaleIsAppliedBeforeRotationAndTranslation)
{
    const edt::Transform transform{
        .translation = {1, 0, 0},
        .rotation = {.yaw = 90.f},
        .scale = {2, 2, 2},
    };

    // Scaled to (2, 0, 0), yawed to (0, 2, 0), then translated by (1, 0, 0).
    ExpectVectorNear(edt::Math::TransformPos(transform.Matrix(), edt::Vec3f{1, 0, 0}), edt::Vec3f{1, 2, 0});
}

TEST(TransformTest, ScaleIsPerAxis)
{
    const edt::Transform transform{.scale = {2, 3, 4}};
    ExpectVectorNear(edt::Math::TransformPos(transform.Matrix(), edt::Vec3f{1, 1, 1}), edt::Vec3f{2, 3, 4});
}
