#pragma once

#include "../Template/TypeHasMethod.h"
#include "TypeFlagUtils.h"
#include "TypeInfo.h"

namespace edt
{
    ImplementHasMethod(GetTypeFlags);

    template<typename T>
    constexpr TypeFlags SelectTypeFlags()
    {
        if constexpr (HasGetTypeFlagsMethod_v<T>)
        {
            return T::GetTypeFlags();
        }
        else
        {
            return GetTypeFlags<T>();
        }
    }

    template<typename T>
    constexpr TypeFlags GetTypeFlags()
    {
        using ApplicableTraits = edt::ValuesList_FilterT<FlagIsDefaultApplicableFilter<T>::template Predicate, AllTypeFlagsList>;
        return FlagsToBitset(ApplicableTraits{});
    }

    template<typename T>
    using TypeInfo = TypeInfoContainer<SelectTypeFlags<T>()>;

    template<typename T>
    TypeInfo<T> GetTypeInfo()
    {
        return TypeInfo<T>{};
    }
}
