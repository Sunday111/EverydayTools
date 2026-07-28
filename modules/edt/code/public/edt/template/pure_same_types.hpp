#pragma once

#include <type_traits>

namespace edt
{
// Every type decays to the same type: cv-qualifiers and references are ignored.
template <typename Head, typename... Tail>
class PureSame : public std::bool_constant<(std::is_same_v<std::decay_t<Head>, std::decay_t<Tail>> && ...)>
{
};

template <typename... Types>
constexpr bool pureSame = PureSame<Types...>::value;
}  // namespace edt
