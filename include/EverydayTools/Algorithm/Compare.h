#pragma once

#include <algorithm>
#include <cmath>
#include <limits>

#include "../Template/WorstReference.h"
#include "../Template/PureSameTypes.h"

namespace edt::detail
{
    class MinIntegerComparator
    {
    public:
        template<typename T1, typename T2>
        constexpr bool operator()(const T1& a, const T2& b) const noexcept {
            return a < b;
        }
    };

    class MaxIntegerComparator
    {
    public:
        template<typename T>
        constexpr bool Compare(const T& a, const T& b) const noexcept {
            return a > b;
        }
    };

    template<typename Comparator, typename Arg0, typename Arg1, typename... Args>
    constexpr WorstReference_t<Arg0, Arg1, Args...>
        CompareValues(Comparator&& comparator, Arg0&& a0, Arg1&& a1, Args&&... args) noexcept {
        if constexpr (sizeof...(Args) == 0) {
            return comparator(a0, a1) ? std::forward<Arg0>(a0) : std::forward<Arg1>(a1);
        } else {
            return CompareValues(std::forward<Comparator>(comparator), std::forward<Arg0>(a0),
                CompareValues(std::forward<Comparator>(comparator), std::forward<Arg1>(a1), std::forward<Args>(args)...));
        }
    }
}

namespace edt
{
    template<typename T>
    constexpr bool Equals(T a, T b, T tolerance = std::numeric_limits<T>::epsilon()) {
        return std::abs(a - b) <= tolerance;
    }

    template<typename Arg0, typename Arg1, typename... Args>
    constexpr decltype(auto) MinInteger(Arg0&& a0, Arg1&& a1, Args&&... args) noexcept {
        using namespace detail;
        static_assert(pureSame<Arg0, Arg1, Args...>);
        const MinIntegerComparator comparator;
        return CompareValues(comparator, std::forward<Arg0>(a0), std::forward<Arg1>(a1), std::forward<Args>(args)...);
    }

    template<typename Arg0, typename Arg1, typename... Args>
    constexpr decltype(auto) MaxInteger(Arg0&& a0, Arg1&& a1, Args&&... args) noexcept {
        using namespace detail;
        static_assert(pureSame<Arg0, Arg1, Args...>);
        const MaxIntegerComparator comparator;
        return CompareValues(comparator, std::forward<Arg0>(a0), std::forward<Arg1>(a1), std::forward<Args>(args)...);
    }
}
