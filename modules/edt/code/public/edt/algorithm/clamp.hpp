#pragma once

#include <algorithm>
#include <concepts>
#include <limits>
#include <utility>

namespace edt
{
template <std::integral Bounds, std::integral From>
[[nodiscard]] constexpr From ClampToAnotherTypeBounds(From value) noexcept
{
    using B = std::numeric_limits<Bounds>;
    using F = std::numeric_limits<From>;

    if constexpr (std::cmp_less(F::lowest(), B::lowest()))
    {
        value = std::max(value, static_cast<From>(B::lowest()));
    }

    if constexpr (std::cmp_greater(F::max(), B::max()))
    {
        value = std::min(value, static_cast<From>(B::max()));
    }

    return value;
}
}  // namespace edt
