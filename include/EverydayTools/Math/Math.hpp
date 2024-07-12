#pragma once

#include <cmath>
#include <concepts>

#include "Constants.hpp"
#include "FloatRange.hpp"
#include "Matrix.hpp"

namespace edt
{
class Math
{
public:
    // returns true if begin <= x < end, i.e x in [begin; end)
    template <std::integral T>
    [[nodiscard]] static constexpr bool InRange(const T& x, const T& begin, const T& end)
    {
        return x >= begin and x < end;
    }

    template <typename T>
    [[nodiscard]] static constexpr T Sqr(T x)
    {
        return x * x;
    }

    [[nodiscard]] static constexpr Mat3f MakeTransform(const FloatRange2D<float>& from, const FloatRange2D<float>& to)
    {
        Mat3f uniform_to_canvas = Mat3f::Identity();
        const float tx = -from.x.begin;
        const float ty = -from.y.begin;
        const float sx = to.x.Extent() / from.x.Extent();
        const float sy = to.y.Extent() / from.y.Extent();
        uniform_to_canvas(0, 0) = sx;
        uniform_to_canvas(1, 1) = sy;
        uniform_to_canvas(0, 2) = tx * sx + to.x.begin;
        uniform_to_canvas(1, 2) = ty * sy + to.y.begin;
        return uniform_to_canvas;
    }

    // Get rainbow colors by time t
    [[nodiscard]] static constexpr Vec3<uint8_t> GetRainbowColors(const float t)
    {
        return {
            static_cast<uint8_t>(255.0f * Sqr(std::sin(t + 0.00f * 2.0f * kPi<float>))),
            static_cast<uint8_t>(255.0f * Sqr(std::sin(t + 0.33f * 2.0f * kPi<float>))),
            static_cast<uint8_t>(255.0f * Sqr(std::sin(t + 0.66f * 2.0f * kPi<float>)))};
    }

    [[nodiscard]] inline static Mat2f MakeRotationMatrix(const float radians)
    {
        auto s = std::sin(radians);
        auto c = std::cos(radians);
        return Mat2f{{c, -s, s, c}};
    }
};

}  // namespace edt
