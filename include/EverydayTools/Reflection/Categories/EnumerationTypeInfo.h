#pragma once

#include <type_traits>
#include "TypeFlagTraits.h"

namespace edt::reflection
{
    class EnumerationTypeInfo
    {
    public:
        int enumTypeInfoMember;
    };

    class EnumerationTypeTraits
    {
    public:
        template<typename T>
        static inline constexpr bool IsTypeApplicable = std::is_enum_v<T>;
        using Container = EnumerationTypeInfo;

        template<typename T>
        static void ConstructContainer(Container&) {

        }
    };

    template<>
    struct TypeFlagToTraitMap<TypeFlags::Enumeration>
    {
        using Trait = EnumerationTypeTraits;
    };
}
