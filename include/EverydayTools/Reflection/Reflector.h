#pragma once

#include "Detail/FwdDecl.h"
#include "Category/CommonTypeInfo.h"
#include "Detail/MakeTypeOperators.h"

namespace edt::reflection
{
    template<typename T>
    class Reflector : public detail::CategoryReflector<T>
    {
    public:
        Reflector(TypeInfo<T>& typeInfo)
			: detail::CategoryReflector<T>(typeInfo)
		{}
	};
}