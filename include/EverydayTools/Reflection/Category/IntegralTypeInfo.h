#pragma once

#include "../FwdDecl.h"
#include "CommonTypeInfo.h"

namespace edt::reflection
{
    class IntegralTypeInfo : public CommonTypeInfo
    {
    public:
        bool isSigned;
    };

    template<typename T>
    class TypeInfo<T, std::enable_if_t<std::is_integral_v<T>>> : public IntegralTypeInfo
    {
    };
}

namespace edt::reflection::detail
{
    template<typename T>
    class CategoryReflector<T, std::enable_if_t<std::is_integral_v<T>>> : public CommonTypeReflector<T>
    {
    public:
        CategoryReflector(TypeInfo<T>& typeInfo)
            : CommonTypeReflector(typeInfo)
        {
            m_typeInfo.category = TypeCategory::Integral;
            m_typeInfo.isSigned = std::is_signed_v<T>;
        }
    };
}
