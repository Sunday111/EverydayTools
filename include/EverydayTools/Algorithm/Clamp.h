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
        constexpr auto max = static_cast<T1>(L2::max());

        if constexpr (std::is_unsigned_v<T1> && std::is_signed_v<T2>)
        {
            return Clamp(T1(0), value, max);
        }
        else
        {
            constexpr auto min = static_cast<T1>(L2::lowest());
            return Clamp(min, value, max);
        }
    }
}
