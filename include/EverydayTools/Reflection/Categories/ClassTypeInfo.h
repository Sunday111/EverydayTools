#pragma once

#include <type_traits>
#include "TypeFlagTraits.h"

namespace edt::reflection
{
    class ClassTypeInfo
    {
    public:
        int classTypeInfoMember;
    };

    class ClassTypeTraits
    {
    public:
        template<typename T>
        static inline constexpr bool IsTypeApplicable = std::is_class_v<T>;
        using Container = ClassTypeInfo;

        static void ConstructContainer(Container&) {

        }
    };

    template<>
    struct TypeFlagToTraitMap<TypeFlags::Class>
    {
        using Trait = ClassTypeTraits;
    };
}
