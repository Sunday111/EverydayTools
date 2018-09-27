#pragma once

namespace edt::reflection
{
	template<typename T, typename Enable = void>
	class TypeInfo;

	template<typename T>
	class Reflector;
}

namespace edt::reflection::detail
{
	template<typename T>
	class CommonTypeReflector;

	template<typename T, typename Enable = void>
	class CategoryReflector
	{};
}