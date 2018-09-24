#pragma once

#include <type_traits>
#include "EverydayTools/Template/TypeHasMethod.h"

#include "FwdDecl.h"
#include "Category/CommonTypeInfo.h"
#include "Category/EnumerationTypeInfo.h"
#include "Category/FloatingPointTypeInfo.h"
#include "Category/IntegralTypeInfo.h"

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

    template<typename T>
    class Reflector : public detail::CategoryReflector<T>
    {
    public:
        Reflector(TypeInfo<T>& typeInfo)
            : detail::CategoryReflector<T>(typeInfo)
        {}
    };

	template<typename T>
	TypeInfo<T> MakeTypeInfo() {
		static_assert(type_is_reflected<T>, "Type is not reflected");
		TypeInfo<T> result;
		if constexpr (detail::HasReflectTypeMethod_v<T>) {
			T::ReflectType(Reflector<T>(result));
		} else {
			ReflectType(Reflector<T>(result));
		}
		return result;
	}
}
