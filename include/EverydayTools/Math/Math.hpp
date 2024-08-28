#pragma once

#include <algorithm>
#include <cmath>
#include <concepts>

#include "Constants.hpp"
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

    // Get rainbow colors by time t
    [[nodiscard]] static constexpr Vec3<uint8_t> GetRainbowColors(const float t)
    {
        return {
            static_cast<uint8_t>(255.0f * Sqr(std::sin(t + 0.00f * 2.0f * kPi<float>))),
            static_cast<uint8_t>(255.0f * Sqr(std::sin(t + 0.33f * 2.0f * kPi<float>))),
            static_cast<uint8_t>(255.0f * Sqr(std::sin(t + 0.66f * 2.0f * kPi<float>)))};
    }

    // Get rainbow colors by time t
    [[nodiscard]] static constexpr Vec4<uint8_t> GetRainbowColorsA(const float t, uint8_t alpha = 255)
    {
        const auto rgb = GetRainbowColors(t);
        return {rgb.x(), rgb.y(), rgb.z(), alpha};
    }

    [[nodiscard]] inline static Mat2f MakeRotationMatrix(const float radians)
    {
        auto s = std::sin(radians);
        auto c = std::cos(radians);
        return Mat2f{{c, -s, s, c}};
    }

    template <typename T, const size_t rows, const size_t columns>
    [[nodiscard]] static constexpr edt::Matrix<T, rows, columns> Clamp(
        const edt::Matrix<T, rows, columns>& v,
        const edt::Matrix<T, rows, columns>& min,
        const edt::Matrix<T, rows, columns>& max)
    {
        edt::Matrix<T, rows, columns> r;
        for (size_t row_index = 0; row_index != rows; ++row_index)
        {
            for (size_t col_index = 0; col_index != columns; ++col_index)
            {
                r(row_index, col_index) =
                    std::clamp(v(row_index, col_index), min(row_index, col_index), max(row_index, col_index));
            }
        }

        return v;
    }

    template <std::floating_point T>
    [[nodiscard]] static constexpr T DegToRad(T degrees)
    {
        return degrees * kPi<T> / 180;
    }

    template <typename T, const size_t rows, const size_t columns>
    [[nodiscard]] static constexpr edt::Matrix<T, rows, columns>
    Clamp(const edt::Matrix<T, rows, columns>& v, T min, T max)
    {
        edt::Matrix<T, rows, columns> r;
        for (size_t row_index = 0; row_index != rows; ++row_index)
        {
            for (size_t col_index = 0; col_index != columns; ++col_index)
            {
                r(row_index, col_index) = std::clamp(v(row_index, col_index), min, max);
            }
        }

        return v;
    }

    static constexpr edt::Mat3f TranslationMatrix(const Vec2f translation)
    {
        auto m = edt::Mat3f::Identity();
        m(0, 2) = translation.x();
        m(1, 2) = translation.y();
        return m;
    }

    static constexpr edt::Mat3f ScaleMatrix(const Vec2f scale)
    {
        auto m = edt::Mat3f::Identity();
        m(0, 0) = scale.x();
        m(1, 1) = scale.y();
        return m;
    }

    static constexpr edt::Mat4f ScaleMatrix(const Vec3f scale)
    {
        auto m = edt::Mat4f::Identity();
        m(0, 0) = scale.x();
        m(1, 1) = scale.y();
        m(2, 2) = scale.z();
        return m;
    }

    [[nodiscard]] static constexpr Vec2f TransformPos(const Mat3f& mat, const Vec2f& pos)
    {
        Vec3f v3 = mat.MatMul(Vec3f{{pos.x(), pos.y(), 1.f}});
        return Vec2f{{v3.x(), v3.y()}};
    }

    [[nodiscard]] static constexpr Vec2f TransformVector(const Mat3f& mat, const Vec2f& vec)
    {
        Vec3f v3 = mat.MatMul(Vec3f{{vec.x(), vec.y(), 0.f}});
        return Vec2f{{v3.x(), v3.y()}};
    }
};

}  // namespace edt
