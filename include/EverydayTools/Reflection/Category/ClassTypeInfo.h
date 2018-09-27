#pragma once

#include "../FwdDecl.h"
#include "CommonTypeInfo.h"

namespace edt::reflection
{
	class ClassTypeInfo : public CommonTypeInfo
	{
	public:
	};

	template<typename T>
	class TypeInfo<T, std::enable_if_t<std::is_class_v<T>>> : public ClassTypeInfo
	{
	};
}