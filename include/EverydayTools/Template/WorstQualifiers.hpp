#pragma once

#include "AnyTypeMatchCondition.hpp"
#include "ApplyIf.hpp"
#include "PureSameTypes.hpp"

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