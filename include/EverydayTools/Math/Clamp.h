#pragma once

#include <algorithm>
#include <type_traits>

namespace edt::math
{
    template<typename T>
    inline T Clamp(T min, T value, T max) {
        value = std::min(value, max);
        value = std::max(value, min);
        return value;
    }

    template<typename T2, typename T1>
    inline T1 ClampToAnotherTypeBounds(T1 value) {
        using L1 = std::numeric_limits<T1>;
        using L2 = std::numeric_limits<T2>;
        constexpr auto t1min = L1::lowest();
        constexpr auto t1max = L1::max();
        constexpr auto t2min = L2::lowest();
        constexpr auto t2max = L2::max();
        constexpr bool t1_limits_are_wider =
            (static_cast<long double>(t1max) > static_cast<long double>(t2max)) &&
            (static_cast<long double>(t1min) < static_cast<long double>(t2min));
        static_assert(t1_limits_are_wider, "ClampToAnotherTypeBounds: Another type must be bigger that argument type");
        return Clamp(static_cast<T1>(t2min), value, static_cast<T1>(t2max));
    }
}
