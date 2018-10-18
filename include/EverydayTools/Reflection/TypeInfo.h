#pragma once

#include "../Template/TypeHasMethod.h"
#include "Detail/TypeInfoContainer.h"

namespace edt::reflection
{
    ImplementHasMethod(GetTypeFlags);

    template<typename T>
    constexpr TypeFlags SelectTypeFlags() {
        if constexpr (HasGetTypeFlagsMethod_v<T>) {
            return T::GetTypeFlags();
        } else {
            return detail::GetTypeFlags<T>();
        }
    }

    template<typename T>
    class TypeInfo : public detail::TypeInfoContainer<SelectTypeFlags<T>()>
    {
    public:
        TypeInfo() {
            Construct(detail::BitsetValuesList<GetTypeFlags()> {});
        }

    protected:
        template<auto... typeFlag>
        void Construct(edt::ValuesList<typeFlag...>) {
            (detail::TypeFlagTraits<typeFlag>::template ConstructContainer<T>(*this), ...);
        }
    };
}
