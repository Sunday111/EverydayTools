#pragma once

#include <type_traits>

namespace edt
{
    class CommonTypeInfo
    {
    public:
        std::size_t size;
    };

    class CommonTypeTraits
    {
    public:
        template<typename T>
        static inline constexpr bool IsTypeApplicable = true;

        using Container = CommonTypeInfo;
    };
}
