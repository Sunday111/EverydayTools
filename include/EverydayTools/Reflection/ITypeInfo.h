#pragma once

#include "../Bitset/EnumBitset.h"

namespace edt::reflection
{
    class NumberTypeInfo;
    class EnumerationTypeInfo;
    class ClassTypeInfo;

    enum class TypeFlags
    {
        None = 0,
        Common = (1 << 0),
        Number = (1 << 1),
        Enumeration = (1 << 3),
        Class = (1 << 4),
    };

    class ITypeInfo
    {
    public:
        virtual TypeFlags                    GetFlags() const = 0;
        virtual const NumberTypeInfo*        CastToNumber() const = 0;
        virtual const EnumerationTypeInfo*   CastToEnumeration() const = 0;
        virtual const ClassTypeInfo*         CastToClass() const = 0;
    };
}

namespace edt
{
    template<>
    struct enable_enum_bitset<edt::reflection::TypeFlags>
    {
        static constexpr bool value = true;
    };
}
