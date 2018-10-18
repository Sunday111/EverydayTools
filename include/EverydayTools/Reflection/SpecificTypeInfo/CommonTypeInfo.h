#pragma once

#include "Operators/MakeCommonOperators.h"

namespace edt::reflection
{
    class CommonTypeInfo
    {
    public:
        std::size_t size;
        CommonOperators operators;
    };
}