#pragma once

#include <type_traits>
#include "TypeFlagTraits.h"
#include "../../SpecificTypeInfo/EnumerationTypeInfo.h"

namespace edt::reflection::detail
{
    template<>
    struct TypeFlagTraits<TypeFlags::Enumeration>
    {
        template<typename T>
        static inline constexpr bool IsTypeApplicable = std::is_enum_v<T>;
        using Container = EnumerationTypeInfo;

        template<typename T>
        static void ConstructContainer(Container&) {

        }
    };
}
