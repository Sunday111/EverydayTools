#pragma once

#include "TypeFlagTraits.h"
#include "../ReflectTypeInterface/CommonReflectTypeInterface.h"

namespace edt::reflection::detail
{
    template<>
    struct TypeFlagTraits<TypeFlags::Common>
    {
        template<typename T>
        static inline constexpr bool IsTypeApplicable = true;

        using Container = CommonReflectTypeInterface;

        template<typename T>
        static void ConstructContainer(Container& c) {
            c.size = sizeof(T);
            c.operators = detail::MakeCommonOperators<T>();
        }
    };
}
