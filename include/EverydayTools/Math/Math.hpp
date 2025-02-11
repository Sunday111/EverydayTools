#pragma once

#include <algorithm>
#include <cmath>
#include <concepts>
#include <cstdint>

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
        return x >= begin && x < end;
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
        return {GetRainbowColors(t), alpha};
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

        return r;
    }

    template <std::floating_point T>
    [[nodiscard]] static constexpr T DegToRad(T degrees)
    {
        return degrees * kPi<T> / 180;
    }

    template <std::floating_point T>
    [[nodiscard]] static constexpr T RadToDeg(T radians)
    {
        return T{180} * radians / kPi<T>;
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

    template <std::floating_point T, size_t rows, size_t columns>
    [[nodiscard]] static constexpr edt::Matrix<T, rows, columns>
    Lerp(const edt::Matrix<T, rows, columns>& a, const edt::Matrix<T, rows, columns>& b, T t)
    {
        edt::Matrix<T, rows, columns> r;
        for (size_t row : r.RowIndices())
        {
            for (size_t column : r.ColumnIndices())
            {
                r(row, column) = std::lerp(a(row, column), b(row, column), t);
            }
        }

        return r;
    }

    static constexpr edt::Mat3f TranslationMatrix(const Vec2f translation)
    {
        auto m = edt::Mat3f::Identity();
        m.SetColumn(2, Vec3f{translation, 1});
        return m;
    }

    static constexpr edt::Mat4f TranslationMatrix(const Vec3f translation)
    {
        auto m = edt::Mat4f::Identity();
        m.SetColumn(3, Vec4f{translation, 1});
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

    static constexpr void SinCos(float angle, float& scalar_sin, float& scalar_cos)
    {
        // Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
        float quotient = (1.f / kPi<float>)*0.5f * angle;
        if (angle >= 0.0f)
        {
            quotient = static_cast<float>(static_cast<int64_t>(quotient + 0.5f));
        }
        else
        {
            quotient = static_cast<float>(static_cast<int64_t>(quotient - 0.5f));
        }
        float y = angle - 2 * kPi<float> * quotient;

        // Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
        float sign;  // NOLINT
        if (y > (kPi<float> / 2.f))
        {
            y = kPi<float> - y;
            sign = -1.0f;
        }
        else if (y < -(kPi<float> / 2.f))
        {
            y = -kPi<float> - y;
            sign = -1.0f;
        }
        else
        {
            sign = +1.0f;
        }

        float y2 = y * y;

        // 11-degree minimax approximation
        scalar_sin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 -
                       0.16666667f) *
                          y2 +
                      1.0f) *
                     y;

        // 10-degree minimax approximation
        float p =
            ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 +
            1.0f;
        scalar_cos = sign * p;
    }

    [[nodiscard]] static constexpr Mat3f RotationMatrix2d(const float angle_radians) noexcept
    {
        float s, c;  // NOLINT
        SinCos(angle_radians, s, c);

        Mat3f m{};
        m.SetRow(0, Vec3f{c, -s, 0});
        m.SetRow(1, Vec3f{s, c, 0});
        m.SetRow(2, Vec3f{0, 0, 1});

        return m;
    }

    [[nodiscard]] static constexpr Mat4f RotationMatrix3dX(const float angle_radians)
    {
        float s, c;  // NOLINT
        SinCos(angle_radians, s, c);

        Mat4f m{};
        m.SetRow(0, Vec4f{1, 0, 0, 0});
        m.SetRow(1, Vec4f{0, c, -s, 0});
        m.SetRow(2, Vec4f{0, s, c, 0});
        m.SetRow(3, Vec4f{0, 0, 0, 1});
        return m;
    }

    [[nodiscard]] static constexpr Mat4f RotationMatrix3dY(const float angle_radians)
    {
        float s, c;  // NOLINT
        SinCos(angle_radians, s, c);
        Mat4f m{};
        m(0, 0) = c;
        m(0, 2) = s;
        m(1, 1) = 1.f;
        m(2, 0) = -s;
        m(2, 2) = c;
        m(3, 3) = 1.f;
        return m;
    }

    [[nodiscard]] static constexpr Mat4f RotationMatrix3dZ(const float angle_radians)
    {
        float s, c;  // NOLINT
        SinCos(angle_radians, s, c);
        Mat4f m{};
        m(0, 0) = c;
        m(0, 1) = -s;
        m(1, 0) = s;
        m(1, 1) = c;
        m(2, 2) = 1.f;
        m(3, 3) = 1.f;
        return m;
    }

    static constexpr void
    ToBasisVectors(const Mat4f& m, Vec3f* x = nullptr, Vec3f* y = nullptr, Vec3f* z = nullptr) noexcept
    {
        if (x) *x = TransformVector(m, Vec3f::AxisX());
        if (y) *y = TransformVector(m, Vec3f::AxisY());
        if (z) *z = TransformVector(m, Vec3f::AxisZ());
    }

    [[nodiscard]] static constexpr Vec2f TransformPos(const Mat3f& mat, const Vec2f& pos)
    {
        Vec3f v3 = mat.MatMul(Vec3f{pos.x(), pos.y(), 1.f});
        return Vec2f{v3.x(), v3.y()};
    }

    [[nodiscard]] static constexpr Vec2f TransformVector(const Mat3f& mat, const Vec2f& vec)
    {
        Vec3f v3 = mat.MatMul(Vec3f{vec.x(), vec.y(), 0.f});
        return Vec2f{v3.x(), v3.y()};
    }

    [[nodiscard]] static constexpr Vec3f TransformPos(const Mat4f& mat, const Vec3f& pos)
    {
        Vec4f v4 = mat.MatMul(Vec4f{pos.x(), pos.y(), pos.z(), 1.f});
        return Vec3f{v4.x(), v4.y(), v4.z()};
    }

    [[nodiscard]] static constexpr Vec3f TransformVector(const Mat4f& mat, const Vec3f& vec)
    {
        Vec4f v4 = mat.MatMul(Vec4f{vec.x(), vec.y(), vec.z(), 0.f});
        return Vec3f{v4.x(), v4.y(), v4.z()};
    }

    template <typename A, typename B>
    [[nodiscard]] static constexpr auto MatMul(A&& a, B&& b)
    {
        return std::forward<A>(a).MatMul(std::forward<B>(b));
    }

    template <typename A, typename B, typename C>
    [[nodiscard]] static constexpr auto MatMul(A&& a, B&& b, C&& c)
    {
        return (std::forward<A>(a).MatMul(std::forward<B>(b))).MatMul(std::forward<C>(c));
    }
};

}  // namespace edt
