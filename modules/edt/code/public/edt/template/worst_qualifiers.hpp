#pragma once

#include "any_type_match_condition.hpp"
#include "apply_if.hpp"
#include "pure_same_types.hpp"

namespace edt
{
template <typename Head, typename... Tail>
class WorstQualifiers
{
private:
    static_assert(pureSame<Head, Tail...>, "Invalid usage");
    static constexpr bool hasConst = anyTypeMatchCondition<std::is_const, Head, Tail...>;
    static constexpr bool hasVolatile = anyTypeMatchCondition<std::is_volatile, Head, Tail...>;
    using AppliedConst = ApplyIf<hasConst, std::add_const_t, Head>;

public:
    using type = ApplyIf<hasVolatile, std::add_volatile_t, AppliedConst>;
};

template <typename... Types>
using WorstQualifiers_t = typename WorstQualifiers<Types...>::type;
}  // namespace edt