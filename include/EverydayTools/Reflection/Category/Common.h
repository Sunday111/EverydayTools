#pragma once

#include "EverydayTools/GUID.h"
#include "../Detail/FwdDecl.h"
#include "../TypeOperators.h"

namespace edt::reflection
{
	enum class TypeCategory
	{
		Unknown,
		Fundamental,
		Enumeration,
		String,
		Array,
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
