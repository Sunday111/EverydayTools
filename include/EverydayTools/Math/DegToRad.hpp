#pragma once

#include <concepts>

#include "Constants.hpp"

namespace edt
{

template <std::floating_point T>
[[nodiscard]] inline constexpr T DegToRad(T degrees)
{
    return degrees * kPi<T> / 180;
}
}  // namespace edt
