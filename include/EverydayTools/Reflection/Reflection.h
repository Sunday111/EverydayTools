#pragma once

#include "TypeFlags.h"
#include "TypeInfo.h"

namespace edt::reflection
{
    template<typename T>
    constexpr TypeFlags GetTypeFlags()
    {
        using ApplicableTraits = edt::ValuesList_FilterT<FlagIsDefaultApplicableFilter<T>::template Predicate, BitsetFlagsList<TypeFlags>>;
        return FlagsToBitset(ApplicableTraits{});
    }

    template<typename T>
    TypeInfo<T> GetTypeInfo()
    {
        return TypeInfo<T>{};
    }
}
