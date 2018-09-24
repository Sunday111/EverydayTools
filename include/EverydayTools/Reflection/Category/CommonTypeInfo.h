#pragma once

#include "EverydayTools/Exception/ThrowIfFailed.h"
#include "EverydayTools/GUID.h"

#include "../FwdDecl.h"
#include "../Operators/TypeOperators.h"
#include "../Operators/MakeCommonOperators.h"

namespace edt::reflection
{
	enum class TypeCategory
	{
		Unknown,
		Integral,
		FloatingPoint,
		Enumeration,
		Class
	};

    class CommonTypeInfo
    {
	public:
		edt::GUID guid;
        TypeOperators operators;
		TypeCategory category;
		size_t size;
    };
}

namespace edt::reflection::detail
{
	template<typename T>
	class CommonTypeReflector
	{
	public:
		CommonTypeReflector(TypeInfo<T>& typeInfo)
			: m_typeInfo(typeInfo)
		{
			m_typeInfo.size = sizeof(T);
			m_typeInfo.operators.common = MakeCommonOperators<T>();
			m_typeInfo.category = TypeCategory::Unknown;
		}

		void SetGUID(const GUID& guid) {
			m_typeInfo.guid = guid;
		}

	protected:
		TypeInfo<T>& m_typeInfo;
	};
}