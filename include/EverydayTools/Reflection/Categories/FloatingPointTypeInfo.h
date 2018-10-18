#pragma once

#include <type_traits>

namespace edt
{
    class FloatingPointTypeInfo
    {
    public:
        int floatTypeInfoMember;
    };

    class FloatingPointTypeTraits
    {
    public:
        template<typename T>
        static inline constexpr bool IsTypeApplicable = std::is_floating_point_v<T>;
        using Container = FloatingPointTypeInfo;
    };
}
