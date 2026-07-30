#pragma once

#include <type_traits>

#include "apply_if.hpp"
#include "pure_same_types.hpp"

namespace edt
{
template <typename Head, typename... Tail>
class WorstQualifiers
{
private:
    static_assert(pureSame<Head, Tail...>, "Invalid usage");
    static constexpr bool hasConst = (std::is_const_v<Head> || ... || std::is_const_v<Tail>);
    static constexpr bool hasVolatile = (std::is_volatile_v<Head> || ... || std::is_volatile_v<Tail>);
    using AppliedConst = ApplyIf<hasConst, std::add_const_t, Head>;

public:
    using type = ApplyIf<hasVolatile, std::add_volatile_t, AppliedConst>;
};

template <typename... Types>
using WorstQualifiers_t = typename WorstQualifiers<Types...>::type;
}  // namespace edt