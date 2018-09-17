#pragma once

#include <type_traits>
#include "Detail/FwdDecl.h"
#include "Category/CommonTypeInfo.h"
#include "Category/EnumerationTypeInfo.h"
#include "Category/FloatingPointTypeInfo.h"
#include "Category/IntegralTypeInfo.h"
#include "Category/StringTypeInfo.h"
#include "TypeIsReflected.h"
#include "Reflector.h"

namespace edt::reflection
{
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
