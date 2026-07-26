#pragma once

#include <concepts>
#include <type_traits>

namespace edt
{

// Branchless conditional swap for integers: exchanges `a` and `b` when
// `condition` is set, leaves them untouched otherwise.
//
// `-condition` is 0 or all-ones (two's complement, guaranteed since C++20), so
// `mask` is either 0 (xor-ing changes nothing) or `a ^ b` (xor-ing performs the
// swap). No data-dependent branch, unlike `if (condition) std::swap(a, b)`.
template <std::integral T>
constexpr void SwapIf(const bool condition, T& a, T& b) noexcept
{
    using U = std::make_unsigned_t<T>;
    const U mask = static_cast<U>((static_cast<U>(a) ^ static_cast<U>(b)) & static_cast<U>(-static_cast<U>(condition)));
    a = static_cast<T>(static_cast<U>(a) ^ mask);
    b = static_cast<T>(static_cast<U>(b) ^ mask);
}

}  // namespace edt
