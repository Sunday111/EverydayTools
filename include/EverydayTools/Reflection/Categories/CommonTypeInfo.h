#pragma once

#include "MakeCommonOperators.h"
#include "TypeFlagTraits.h"

namespace edt::reflection
{
    class CommonTypeInfo
    {
    public:
        std::size_t size;
        CommonOperators operators;
    };

    class CommonTypeTraits
    {
    public:
        template<typename T>
        static inline constexpr bool IsTypeApplicable = true;

        using Container = CommonTypeInfo;

        template<typename T>
        static void ConstructContainer(Container& c) {
            c.size = sizeof(T);
            c.operators = detail::MakeCommonOperators<T>();
        }
    };

    template<>
    struct TypeFlagToTraitMap<TypeFlags::Common>
    {
        using Trait = CommonTypeTraits;
    };
}
