#pragma once

#include "EverydayTools/Template/TypeHasMethod.h"
#include "Detail/FwdDecl.h"
#include "Reflector.h"

namespace edt::reflection::detail
{
    ImplementHasMethod(ReflectType);

    template<typename T>
    constexpr bool HasReflectTypeOverload(...) {
        return false;
    }

    template<typename T>
    constexpr auto HasReflectTypeOverload(int)
        -> decltype(ReflectType(std::declval<Reflector<T>>()), bool())
    {
        return true;
    }
}

namespace edt::reflection
{
    template<typename T>
    constexpr inline bool type_is_reflected = detail::HasReflectTypeMethod_v<T> || detail::HasReflectTypeOverload<T>(0);
}
