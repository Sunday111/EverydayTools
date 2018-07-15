#pragma once

#include "AllTypesMatchCondition.h"

namespace edt::detail
{
    template<typename MainType>
    class IsPureSameTrait
    {
    public:
        template<typename T>
        using Trait = std::is_same<std::decay_t<MainType>, std::decay_t<T>>;
    };
}

namespace edt
{
    template<typename Head, typename... Tail>
    class PureSame : public AllTypesMatchCondition<
        typename detail::IsPureSameTrait<Head>::Trait, Tail...>
    {
    };

    template<typename... Types>
    constexpr bool pureSame = PureSame<Types...>::value;
}
