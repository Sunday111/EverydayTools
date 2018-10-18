#pragma once

#include "Categories/TypeTraitMap.h"
#include "TypeFlags.h"
#include "../Template/TypesList.h"
#include "../Template/ValuesList.h"

namespace edt
{
    template<auto index>
    struct TypeFlagByIndex
    {
        static constexpr auto Value = MakeFlagByIndex<TypeFlags, index>();
    };

    template<auto flag>
    struct TypeFlagHasTrait
    {
        static constexpr bool Value = !std::is_same_v<TypeTraitMap<flag>, InvalidTypeTrait>;
    };

    using AllTypeFlagsList = ValuesList_FilterT<TypeFlagHasTrait,
        ValuesList_MapToValueT<TypeFlagByIndex,
        ValuesList_FromIntegerSequenceT<
        std::make_index_sequence<SizeInBits<TypeFlags>>>>>;
    using AllTypeFlagTraitsList = ValuesList_MapToTypeT<TypeTraitMap, AllTypeFlagsList>;

    template<auto bitset>
    struct FlagIsSetFilter
    {
        template<auto flag>
        struct Functor
        {
            static constexpr bool Value = FlagIsSet(bitset, flag);
        };
    };

    template<auto bitset>
    using BitsetValuesList = ValuesList_FilterT<FlagIsSetFilter<bitset>::template Functor, AllTypeFlagsList>;

    template<auto bitset>
    using BitsetTraitsList = ValuesList_MapToTypeT<TypeTraitMap, BitsetValuesList<bitset>>;

    template<typename T>
    struct FlagIsDefaultApplicableFilter
    {
        template<auto flag>
        struct Predicate
        {
            static constexpr bool Value = TypeTraitMap<flag>::template IsTypeApplicable<T>;
        };
    };

    template<auto... values>
    constexpr auto FlagsToBitset(edt::ValuesList<values...>) {
        return (values | ...);
    }
}