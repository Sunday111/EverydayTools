#pragma once

#include "../TypeFlags.h"

namespace edt::reflection
{
    struct InvalidTypeTrait
    {
    };

    template<TypeFlags flag>
    struct TypeFlagToTraitMap
    {
        using Trait = InvalidTypeTrait;
    };
}