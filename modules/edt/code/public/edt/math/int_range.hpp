#pragma once

#include <concepts>
#include <cstddef>
#include <limits>
#include <ranges>
#include <tuple>

#include "math.hpp"
#include "matrix.hpp"

namespace edt
{

template <std::integral T>
class IntRange
{
public:
    [[nodiscard]] constexpr bool Contains(const T& value) const noexcept { return Math::InRange(value, begin, end); }

    [[nodiscard]] constexpr T Extent() const { return end - begin; }

    [[nodiscard]] constexpr std::tuple<IntRange, IntRange> Split(std::size_t left_side_size) const
    {
        const T split_point = begin + static_cast<T>(left_side_size);
        return {
            {.begin = begin, .end = split_point},
            {.begin = split_point, .end = end},
        };
    }

    [[nodiscard]] constexpr auto Iterable() const { return std::views::iota(begin, end); }

    T begin = std::numeric_limits<T>::lowest();
    T end = std::numeric_limits<T>::max();
};

template <std::integral T>
class IntRange2D
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

    [[nodiscard]] constexpr Vec2<T> Extent() const { return {x.Extent(), y.Extent()}; }

    [[nodiscard]] static constexpr IntRange2D FromBeginAndExtent(const Vec2<T>& begin, const Vec2<T>& extent)
    {
        const auto end = begin + extent;
        return {
            .x = {.begin = begin.x(), .end = end.x()},
            .y = {.begin = begin.y(), .end = end.y()},
        };
    }

    IntRange<T> x{};
    IntRange<T> y{};
};
}  // namespace edt
