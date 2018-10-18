#pragma once

#include "TypeFlagTraits.h"
#include "../../SpecificTypeInfo/CommonTypeInfo.h"

namespace edt::reflection::detail
{
    template<>
    struct TypeFlagTraits<TypeFlags::Common>
    {
        template<typename T>
        static inline constexpr bool IsTypeApplicable = true;

        using Container = CommonTypeInfo;

        template<typename T>
        static void ConstructContainer(Container& c) {
            c.size = sizeof(T);
            c.operators = detail::MakeCommonOperators<T>();
        }
    };
}
