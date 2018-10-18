#pragma once

#include "CommonTypeInfo.h"
#include "IntegralTypeInfo.h"
#include "FloatingPointTypeInfo.h"
#include "EnumerationTypeInfo.h"
#include "ClassTypeInfo.h"

namespace edt
{
    struct InvalidTypeTrait
    {
    };

    template<TypeFlags flag>
    struct TypeFlagToTraitMap
    {
        using Trait = InvalidTypeTrait;
    };

    template<>
    struct TypeFlagToTraitMap<TypeFlags::Common>
    {
        using Trait = CommonTypeTraits;
    };

    template<>
    struct TypeFlagToTraitMap<TypeFlags::Integral>
    {
        using Trait = IntegralTypeTraits;
    };

    template<>
    struct TypeFlagToTraitMap<TypeFlags::FloatingPoint>
    {
        using Trait = FloatingPointTypeTraits;
    };

    template<>
    struct TypeFlagToTraitMap<TypeFlags::Enumeration>
    {
        using Trait = EnumerationTypeTraits;
    };

    template<>
    struct TypeFlagToTraitMap<TypeFlags::Class>
    {
        using Trait = ClassTypeTraits;
    };

    template<auto flag>
    using TypeTraitMap = typename TypeFlagToTraitMap<flag>::Trait;
}
