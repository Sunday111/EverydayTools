#include "EverydayTools/Math/Math.hpp"

#include "TestTools.hpp"
#include <numbers>

namespace edt
{
static_assert(
    []
    {
        float s{}, c{};
        edt::Math::SinCos(edt::Math::DegToRad(0.f), s, c);
        ExpectEq(s, 0);
        ExpectEq(c, 1);

        edt::Math::SinCos(edt::Math::DegToRad(30.f), s, c);
        ExpectEq(s, 0.5f);
        ExpectEq(c, std::numbers::sqrt3_v<float> / 2);

        edt::Math::SinCos(edt::Math::DegToRad(45.f), s, c);
        ExpectEq(s, 1.f / std::numbers::sqrt2_v<float>);
        ExpectEq(c, 1.f / std::numbers::sqrt2_v<float>);

        edt::Math::SinCos(edt::Math::DegToRad(60.f), s, c);
        ExpectEq(s, std::numbers::sqrt3_v<float> / 2);
        ExpectEq(c, 0.5f);

        edt::Math::SinCos(edt::Math::DegToRad(90.f), s, c);
        ExpectEq(s, 1.f);
        ExpectEq(c, 0.f);

        edt::Math::SinCos(edt::Math::DegToRad(120.f), s, c);
        ExpectEq(s, std::numbers::sqrt3_v<float> / 2);
        ExpectEq(c, -0.5f);

        edt::Math::SinCos(edt::Math::DegToRad(180.f), s, c);
        ExpectEq(s, 0);
        ExpectEq(c, -1);

        edt::Math::SinCos(edt::Math::DegToRad(270.f), s, c);
        ExpectEq(s, -1);
        ExpectEq(c, 0);

        edt::Math::SinCos(edt::Math::DegToRad(360.f), s, c);
        ExpectEq(s, 0);
        ExpectEq(c, 1);

        return true;
    }(),
    "SinCos test");

static_assert(
    []
    {
        auto m = Math::TranslationMatrix(Vec2f{9, 3});
        ExpectEq(m.GetRow(0), {1, 0, 9});
        ExpectEq(m.GetRow(1), {0, 1, 3});
        ExpectEq(m.GetRow(2), {0, 0, 1});
        return true;
    }(),
    "2d translation matrix test");

static_assert(
    []
    {
        auto m = Math::TranslationMatrix(Vec3f{9, 3, 6});
        ExpectEq(m.GetRow(0), {1, 0, 0, 9});
        ExpectEq(m.GetRow(1), {0, 1, 0, 3});
        ExpectEq(m.GetRow(2), {0, 0, 1, 6});
        ExpectEq(m.GetRow(3), {0, 0, 0, 1});
        return true;
    }(),
    "3d translation matrix test");

static_assert(
    []
    {
        auto m = Math::ScaleMatrix(Vec2f{9, 3});
        ExpectEq(m.GetRow(0), {9, 0, 0});
        ExpectEq(m.GetRow(1), {0, 3, 0});
        ExpectEq(m.GetRow(2), {0, 0, 1});
        return true;
    }(),
    "2d scale matrix test");

static_assert(
    []
    {
        auto m = Math::ScaleMatrix(Vec3f{9, 3, 6});
        ExpectEq(m.GetRow(0), {9, 0, 0, 0});
        ExpectEq(m.GetRow(1), {0, 3, 0, 0});
        ExpectEq(m.GetRow(2), {0, 0, 6, 0});
        ExpectEq(m.GetRow(3), {0, 0, 0, 1});
        return true;
    }(),
    "3d scale matrix test");

static_assert(
    []
    {
        auto m = Math::RotationMatrix2d(Math::DegToRad(30.f));
        ExpectEq(m.GetRow(0), {std::numbers::sqrt3_v<float> / 2, -0.5f, 0});
        ExpectEq(m.GetRow(1), {0.5f, std::numbers::sqrt3_v<float> / 2, 0});
        ExpectEq(m.GetRow(2), {0, 0, 1});

        auto v = Math::TransformVector(Math::RotationMatrix2d(Math::DegToRad(45.f)), Vec2f{1.f, 0.f});
        ExpectEq(v, {1 / std::numbers::sqrt2_v<float>, 1 / std::numbers::sqrt2_v<float>});

        return true;
    }(),
    "2d rotation matrix test");

static_assert(
    []
    {
        auto m = Math::RotationMatrix3dX(Math::DegToRad(30.f));
        ExpectEq(m.GetRow(0), {1, 0, 0, 0});
        ExpectEq(m.GetRow(1), {0, std::numbers::sqrt3_v<float> / 2, -0.5f, 0});
        ExpectEq(m.GetRow(2), {0, 0.5f, std::numbers::sqrt3_v<float> / 2, 0});
        ExpectEq(m.GetRow(3), {0, 0, 0, 1});

        auto v = Math::TransformVector(Math::RotationMatrix3dX(Math::DegToRad(45.f)), Vec3f{0, 1, 0});
        ExpectEq(v, {0.f, 1 / std::numbers::sqrt2_v<float>, 1 / std::numbers::sqrt2_v<float>});

        return true;
    }(),
    "3d rotation matrix around axis x");
}  // namespace edt
