#pragma once

#include <concepts>

#include "constants.hpp"

namespace edt {

template <std::floating_point T>
[[nodiscard]] inline constexpr T DegToRad(T degrees) {
  return degrees * PI<T> / 180;
}
}  // namespace edt