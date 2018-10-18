#pragma once

#include "../Bitset/EnumBitset.h"
#include "../Bitset/BitsetUtilities.h"
#include "../Size.h"
#include "../Template/ValuesList.h"

namespace edt::reflection
{
    enum class TypeFlags
    {
        None = 0,
        Common = (1 << 0),
        Number = (1 << 1),
        Enumeration = (1 << 3),
        Class = (1 << 4),
    };

    template<auto... values>
    constexpr auto FlagsToBitset(edt::ValuesList<values...>) {
        return (values | ...);
    }
}

namespace edt
{
    template<>
    struct enable_enum_bitset<edt::reflection::TypeFlags>
    {
        static constexpr bool value = true;
    };
}
