#pragma once

#include "AllTypesMatchCondition.hpp"

namespace edt::detail
{
template <typename MainType>
class IsPureSameTrait
{
public:
    template <typename T>
    using Trait = std::is_same<std::decay_t<MainType>, std::decay_t<T>>;
};
}  // namespace edt::detail

namespace edt
{
template <typename Head, typename... Tail>
class PureSame : public AllTypesMatchCondition<detail::IsPureSameTrait<Head>::template Trait, Tail...>
{
};

template <typename... Types>
constexpr bool pureSame = PureSame<Types...>::value;
}  // namespace edt
