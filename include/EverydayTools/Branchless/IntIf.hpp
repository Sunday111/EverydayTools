#pragma once

#include <concepts>
#include <type_traits>

namespace edt
{

// Branchless conditional select for integers: returns `if_true` when
// `condition` is set, otherwise `if_false`.
//
// Unlike the ternary operator (whose branchlessness is at the mercy of the
// optimizer) this always computes both results with pure bit arithmetic, so
// there is never a data-dependent branch. That also means both operands are
// always evaluated - only pass cheap, side-effect-free values.
//
// How it works: `-condition` is 0 when the condition is unset and all-ones when
// it is set (two's complement is guaranteed since C++20). AND-ing the xor of the
// two values with that mask, then xor-ing back into `if_false`, yields `if_true`
// exactly when the mask is all-ones.
template <std::integral T>
[[nodiscard]] constexpr T IntIf(const bool condition, const T if_true, const std::type_identity_t<T> if_false) noexcept
{
    using U = std::make_unsigned_t<T>;
    const U mask = static_cast<U>(-static_cast<U>(condition));
    const U diff = static_cast<U>(static_cast<U>(if_true) ^ static_cast<U>(if_false));
    return static_cast<T>(static_cast<U>(if_false) ^ (diff & mask));
}

}  // namespace edt
