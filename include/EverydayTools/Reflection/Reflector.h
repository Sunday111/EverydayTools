#pragma once

#include "Detail/FwdDecl.h"
#include "Category/Common.h"
#include "Detail/MakeTypeOperators.h"

namespace edt::reflection::detail
{
	template<typename T>
	class CommonTypeReflector
	{
	public:
		CommonTypeReflector(TypeInfo<T>& typeInfo);

		void SetGUID(const GUID& guid);

	protected:
		TypeInfo<T>& m_typeInfo;
	};

	template<typename T>
	CommonTypeReflector<T>::CommonTypeReflector(TypeInfo<T>& typeInfo)
		: m_typeInfo(typeInfo)
	{
		m_typeInfo.size = sizeof(T);
		m_typeInfo.operators = MakeTypeOperators<T>();
		m_typeInfo.category = TypeCategory::Unknown;
	}

	template<typename T>
	void CommonTypeReflector<T>::SetGUID(const GUID& guid) {
		m_typeInfo.guid = guid;
	}

	template<typename T, typename Enable = void>
	class CategoryReflector
	{

	};

	template<typename T>
	class CategoryReflector<T, std::enable_if_t<std::is_integral_v<T>>> : public CommonTypeReflector<T>
	{
	public:
		CategoryReflector(TypeInfo<T>& typeInfo)
			: CommonTypeReflector(typeInfo)
		{
			m_typeInfo.category = TypeCategory::Fundamental;
			m_typeInfo.fundamentalCategory = FundamentalTypeCategory::Integral;
			m_typeInfo.isSigned = std::is_signed_v<T>;
		}
	};

	template<typename T>
	class CategoryReflector<T, std::enable_if_t<std::is_floating_point_v<T>>> : public CommonTypeReflector<T>
	{
	public:
		CategoryReflector(TypeInfo<T>& typeInfo)
			: CommonTypeReflector(typeInfo)
		{
			m_typeInfo.category = TypeCategory::Fundamental;
			m_typeInfo.fundamentalCategory = FundamentalTypeCategory::FloatingPoint;
		}
	};
}

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