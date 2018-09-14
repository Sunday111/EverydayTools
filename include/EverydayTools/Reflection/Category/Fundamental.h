#pragma once

#include "../Detail/FwdDecl.h"
#include "Common.h"

namespace edt::reflection
{
	enum class FundamentalTypeCategory
	{
		Integral,
		FloatingPoint
	};

	class FundamentalTypeInfo : public CommonTypeInfo
	{
	public:
		FundamentalTypeCategory fundamentalCategory;
	};

	class IntegralTypeInfo : public FundamentalTypeInfo
	{
	public:
		bool isSigned;
	};

	class FloatingPointTypeInfo : public FundamentalTypeInfo
	{
	public:
	};
}