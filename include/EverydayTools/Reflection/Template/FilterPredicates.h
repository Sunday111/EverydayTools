#pragma once

#include "../ForwardDeclaration.h"
#include "../../Bitset/BitsetUtilities.h"

namespace edt::reflection
{
    template<typename T>
    struct FlagIsDefaultApplicableFilter
    {
        template<auto flag>
        struct Predicate
        {
            static constexpr bool Value = TypeTraitMap<flag>::template IsTypeApplicable<T>;
        };
    };
}