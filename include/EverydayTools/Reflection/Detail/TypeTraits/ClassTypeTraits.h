#pragma once

#include <type_traits>
#include "TypeFlagTraits.h"
#include "../ReflectTypeInterface/ClassReflectTypeInterface.h"

namespace edt::reflection::detail
{
    template<>
    struct TypeFlagTraits<TypeFlags::Class>
    {
        template<typename T>
        static inline constexpr bool IsTypeApplicable = std::is_class_v<T>;
        using Container = ClassReflectTypeInterface;

        static void ConstructContainer(Container&) {

        }
    };
}
