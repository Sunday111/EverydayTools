#pragma once

#include <type_traits>

namespace edt
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
    };
}
