#pragma once

#include "../Detail/FwdDecl.h"
#include "CommonTypeInfo.h"

namespace edt::reflection
{
	template<typename T>
	struct StringInterface
	{
	};

	class StringTypeInfo : public CommonTypeInfo
	{
	public:
	};

	template<typename T>
	class TypeInfo<T, typename StringInterface<T>::Type> : public StringTypeInfo
	{
	};
}

namespace edt::reflection::detail
{
	template<typename T>
	class CategoryReflector<T, typename StringInterface<T>::Type> : public CommonTypeReflector<T>
	{
	public:
		CategoryReflector(TypeInfo<T>& typeInfo)
			: CommonTypeReflector(typeInfo)
		{
			m_typeInfo.category = TypeCategory::String;
		}
	};
}
