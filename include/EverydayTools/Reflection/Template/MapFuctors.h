#pragma once

#include "../ForwardDeclaration.h"

namespace edt::reflection
{

    template<auto flag>
    struct TypeFlagHasTrait
    {
        static constexpr bool Value = !std::is_same_v<TypeTraitMap<flag>, InvalidTypeTrait>;
    };
}