#pragma once

#include <concepts>
#include <utility>

#include "throw_if_failed.hpp"

namespace edt
{
template <std::integral To, std::integral From>
[[nodiscard]] constexpr To CheckedCast(From from)
{
    ThrowIfFailed(std::in_range<To>(from), "Type overflow");
    return static_cast<To>(from);
}
}  // namespace edt
