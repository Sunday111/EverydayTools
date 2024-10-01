#pragma once

#include <algorithm>

#include "Matrix.hpp"

namespace edt
{

template <std::floating_point T>
class FloatRange
{
public:
    [[nodiscard]] constexpr T Extent() const { return end - begin; }

    [[nodiscard]] constexpr bool Contains(float v) const { return v >= begin && v <= end; }

    constexpr void EnlargeLeft(const float v) { begin -= v; }
    constexpr void EnlargeRight(const float v) { end += v; }
    constexpr void Enlarge(const float value)
    {
        EnlargeLeft(value);
        EnlargeRight(value);
    }
    [[nodiscard]] constexpr FloatRange Enlarged(const float value) const
    {
        auto copy = *this;
        copy.Enlarge(value);
        return copy;
    }

    [[nodiscard]] constexpr T Clamp(const float value) const { return std::clamp(value, begin, end); }

    constexpr void Shift(const float value)
    {
        begin += value;
        end += value;
    }

    [[nodiscard]] constexpr FloatRange Shifted(const float value) const
    {
        auto copy = *this;
        copy.Shift(value);
        return copy();
    }

    T begin = std::numeric_limits<T>::lowest();
    T end = std::numeric_limits<T>::max();
};

template <std::floating_point T>
class FloatRange2D
{
public:
    [[nodiscard]] constexpr bool Contains(const std::tuple<T, T>& p) const noexcept
    {
        return x.Contains(std::get<0>(p)) && y.Contains(std::get<1>(p));
    }
    [[nodiscard]] constexpr bool Contains(const T& vx, const T& vy) const noexcept
    {
        return x.Contains(vx) && y.Contains(vy);
    }
    [[nodiscard]] constexpr Vec2<T> Extent() const noexcept { return Vec2<T>{x.Extent(), y.Extent()}; }

    [[nodiscard]] constexpr Vec2<T> Min() const noexcept { return {x.begin, y.begin}; }

    [[nodiscard]] constexpr Vec2<T> Max() const noexcept { return {x.end, y.end}; }

    [[nodiscard]] constexpr Vec2<T> Uniform(const float v) const noexcept { return Uniform(Vec2<T>{v, v}); }

    [[nodiscard]] constexpr Vec2<T> Uniform(const Vec2<T>& v) const noexcept { return Min() + v * Extent(); }

    [[nodiscard]] static constexpr FloatRange2D FromMinMax(const Vec2<T>& min, const Vec2<T> max)
    {
        return {{min.x(), max.x()}, {min.y(), max.y()}};
    }

    constexpr void Shift(const Vec2f value)
    {
        x.Shift(value.x());
        y.Shift(value.y());
    }

    [[nodiscard]] constexpr FloatRange2D Shifted(const Vec2f value) const
    {
        auto copy = *this;
        copy.Shift(value);
        return copy;
    }

    constexpr void Enlarge(const float value)
    {
        x.Enlarge(value);
        y.Enlarge(value);
    }

    constexpr void Enlarge(const Vec2f value)
    {
        x.Enlarge(value.x());
        y.Enlarge(value.y());
    }

    [[nodiscard]] constexpr FloatRange2D Enlarged(const float value) const
    {
        auto copy = *this;
        copy.Enlarge(value);
        return copy;
    }

    [[nodiscard]] constexpr FloatRange2D Enlarged(const Vec2f value) const
    {
        auto copy = *this;
        copy.Enlarge(value);
        return copy;
    }

    [[nodiscard]] constexpr Vec2<T> Clamp(const Vec2<T>& v) const { return Vec2<T>{x.Clamp(v.x()), y.Clamp(v.y())}; }

    FloatRange<T> x{};
    FloatRange<T> y{};
};

using FloatRange2Df = FloatRange2D<float>;
}  // namespace edt
