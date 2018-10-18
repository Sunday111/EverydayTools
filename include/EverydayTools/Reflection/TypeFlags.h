#pragma once

#include "../Bitset/EnumBitset.h"
#include "../Bitset/BitsetUtilities.h"
#include "../Size.h"


namespace edt
{
    enum class TypeFlags
    {
        None = 0,
        Common = (1 << 0),
        Integral = (1 << 1),
        FloatingPoint = (1 << 2),
        Enumeration = (1 << 3),
        Class = (1 << 4),
    };

    template<>
    struct enable_enum_bitset<TypeFlags>
    {
        static constexpr bool value = true;
    };
}
