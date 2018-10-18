#pragma once

#include "CommonTypeInfo.h"
#include "NumberTypeInfo.h"
#include "EnumerationTypeInfo.h"
#include "ClassTypeInfo.h"

namespace edt::reflection
{
    template<auto flag>
    using TypeTraitMap = typename TypeFlagToTraitMap<flag>::Trait;
}
