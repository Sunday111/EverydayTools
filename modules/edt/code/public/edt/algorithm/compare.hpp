#pragma once

#include <concepts>
#include <limits>

namespace edt
{
template <std::floating_point T>
[[nodiscard]] constexpr bool AlmostEqual(T a, T b, T tolerance = std::numeric_limits<T>::epsilon())
{
    const T delta = a - b;
    return (delta < 0 ? -delta : delta) <= tolerance;
}
}  // namespace edt
