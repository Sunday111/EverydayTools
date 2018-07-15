#pragma once

#include <algorithm>
#include <type_traits>

namespace edt
{
    template<typename T>
    inline T Clamp(T min, T value, T max) {
        value = std::min(value, max);
        value = std::max(value, min);
        return value;
    }

    template<typename T2, typename T1>
    inline T1 ClampToAnotherTypeBounds(T1 value) {
        using L2 = std::numeric_limits<T2>;
        constexpr auto min = static_cast<T1>(L2::lowest());
        constexpr auto max = static_cast<T1>(L2::max());
        return Clamp(min, value, max);
    }
}
