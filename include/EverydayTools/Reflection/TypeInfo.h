#pragma once

#include <type_traits>
#include "Detail/FwdDecl.h"
#include "Category/Common.h"
#include "Category/Fundamental.h"
#include "TypeIsReflected.h"
#include "Reflector.h"

namespace edt::reflection
{
	template<typename T, typename Enable>
	class TypeInfo { };

	template<typename T>
	class TypeInfo<T, std::enable_if_t<std::is_integral_v<T>>> : public IntegralTypeInfo {};

	template<typename T>
	class TypeInfo<T, std::enable_if_t<std::is_floating_point_v<T>>> : public FloatingPointTypeInfo {};

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
