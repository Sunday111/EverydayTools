#pragma once

#include "../FwdDecl.h"
#include "CommonTypeInfo.h"

namespace edt::reflection
{
    class FloatingPointTypeInfo : public CommonTypeInfo
    {
    public:
    };

    template<typename T>
    class TypeInfo<T, std::enable_if_t<std::is_floating_point_v<T>>> : public FloatingPointTypeInfo
    {
    };
}

namespace edt::reflection::detail
{
    template<typename T>
    class CategoryReflector<T, std::enable_if_t<std::is_floating_point_v<T>>> : public CommonTypeReflector<T>
    {
    public:
        CategoryReflector(TypeInfo<T>& typeInfo)
            : CommonTypeReflector<T>(typeInfo)
        {
            this->m_typeInfo.category = TypeCategory::FloatingPoint;
        }
    };
}