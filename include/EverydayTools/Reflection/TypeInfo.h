#pragma once

#include "../Template/TypeHasMethod.h"
#include "TypeInfoContainer.h"

namespace edt::reflection
{
    ImplementHasMethod(GetTypeFlags);

    template<typename T>
    constexpr TypeFlags SelectTypeFlags() {
        if constexpr (HasGetTypeFlagsMethod_v<T>) {
            return T::GetTypeFlags();
        } else {
            return GetTypeFlags<T>();
        }
    }

    template<typename T>
    class TypeInfo : public TypeInfoContainer<SelectTypeFlags<T>()>
    {
    public:
        TypeInfo() {
            Construct(BitsetValuesList<GetTypeFlags()> {});
        }

    protected:
        template<auto... typeFlag>
        void Construct(edt::ValuesList<typeFlag...>) {
            (FlagTraits<typeFlag>::template ConstructContainer<T>(*this), ...);
        }
    };
}
