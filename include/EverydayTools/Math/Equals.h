#pragma once

#include <cmath>
#include <limits>

namespace edt::math
{
    template<typename T>
    constexpr bool Equals(T a, T b, T tolerance = std::numeric_limits<T>::epsilon()) {
        return std::abs(a - b) <= tolerance;
    }
}
