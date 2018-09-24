#pragma once

#include "../Detail/FwdDecl.h"
#include "CommonTypeInfo.h"

namespace edt::reflection
{
    class IString
    {
    public:
        virtual const char* GetValue() const = 0;
    };

    template<typename T>
    inline constexpr bool HasStringInterface = std::is_base_of_v<IString, T>;

	class StringTypeInfo : public CommonTypeInfo
	{
	public:
	};

	template<typename T>
	class TypeInfo<T, std::enable_if_t<HasStringInterface<T>>> : public StringTypeInfo
	{

	};
}

namespace edt::reflection::detail
{
	template<typename T>
	class CategoryReflector<T, std::enable_if_t<HasStringInterface<T>>> : public CommonTypeReflector<T>
	{
	public:
		CategoryReflector(TypeInfo<T>& typeInfo)
			: CommonTypeReflector(typeInfo)
		{
			m_typeInfo.category = TypeCategory::String;
		}
	};
}
