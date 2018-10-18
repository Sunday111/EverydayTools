#pragma once

#include <type_traits>

namespace edt
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
    };
}
