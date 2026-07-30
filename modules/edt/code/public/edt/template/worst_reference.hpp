#pragma once

#include <type_traits>

#include "worst_qualifiers.hpp"

namespace edt
{
template <typename Head, typename... Tail>
class WorstReference
{
    using WQ = WorstQualifiers_t<Head, Tail...>;
    static constexpr bool rValueReference =
        (std::is_rvalue_reference_v<Head> && ... && std::is_rvalue_reference_v<Tail>);
    static constexpr bool lValueReference =
        (std::is_lvalue_reference_v<Head> && ... && std::is_lvalue_reference_v<Tail>);
    using LValueApplied = ApplyIf<lValueReference, std::add_lvalue_reference_t, WQ>;

public:
    using type = ApplyIf<rValueReference, std::add_rvalue_reference_t, LValueApplied>;
};

template <typename... Types>
using WorstReference_t = typename WorstReference<Types...>::type;
}  // namespace edt