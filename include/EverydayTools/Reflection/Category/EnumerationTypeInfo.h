#pragma once

#include <string>
#include <vector>

#include "../Detail/FwdDecl.h"
#include "CommonTypeInfo.h"

namespace edt::reflection
{
	class EnumerationTypeInfo : public CommonTypeInfo
	{
	public:
		class EnumerationEntryInfo
		{
		public:
			size_t value;
			std::string name;
		};

		size_t GetConstantsCount() const {
			return m_constants.size();
		}

		const EnumerationEntryInfo& GetConstantInfo(size_t index) const {
			edt::ThrowIfFailed(index < GetConstantsCount());
			return m_constants[index];
		}

	public:
		std::vector<EnumerationEntryInfo> m_constants;
	};

	template<typename T>
	class TypeInfo<T, std::enable_if_t<std::is_enum_v<T>>> : public EnumerationTypeInfo
	{
	};
}

namespace edt::reflection::detail
{
	template<typename T>
	class CategoryReflector<T, std::enable_if_t<std::is_enum_v<T>>> : public CommonTypeReflector<T>
	{
	public:
		CategoryReflector(TypeInfo<T>& typeInfo)
			: CommonTypeReflector(typeInfo)
		{
			m_typeInfo.category = TypeCategory::Enumeration;
		}

		void AddConstant(T value, const char* name) {
			m_typeInfo.m_constants.push_back(typename EnumerationTypeInfo::EnumerationEntryInfo {
				static_cast<size_t>(value),
				std::string(name)
				});
		}
	};
}
