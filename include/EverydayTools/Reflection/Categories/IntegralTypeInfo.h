#pragma once

#include <type_traits>

namespace edt
{
    class IntegralTypeInfo
    {
    public:
        int intTypeInfoMember;
    };

    class IntegralTypeTraits
    {
    public:
        template<typename T>
        static inline constexpr bool IsTypeApplicable = std::is_integral_v<T>;
        using Container = IntegralTypeInfo;
    };
}
