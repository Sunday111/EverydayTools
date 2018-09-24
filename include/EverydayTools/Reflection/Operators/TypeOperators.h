#pragma once

#include "ArrayOperators.h"
#include "CommonOperators.h"
#include "StringOperators.h"

namespace edt::reflection
{
    class TypeOperators
    {
    public:
        CommonOperators common;
        ArrayOperators array;
        StringOperators string;
    };
}