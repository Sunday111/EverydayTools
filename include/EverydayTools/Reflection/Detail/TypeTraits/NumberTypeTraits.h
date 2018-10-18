#pragma once

#include <type_traits>
#include "TypeFlagTraits.h"
#include "../../SpecificTypeInfo/NumberTypeInfo.h"

namespace edt::reflection::detail
{
    template<>
    struct TypeFlagTraits<TypeFlags::Number>
    {
        template<typename T>
        static inline constexpr bool IsTypeApplicable = std::is_arithmetic_v<T>;
        using Container = NumberTypeInfo;

        template<typename T>
        static void ConstructContainer(Container& c) {
            if constexpr (std::is_floating_point_v<T>) {
                c.numberType = NumberType::FloatingPoint;
            }
            else {
                if constexpr (std::is_signed_v<T>) {
                    c.numberType = NumberType::SignedInteger;
                }
                else {
                    c.numberType = NumberType::UnsignedInteger;
                }
            }
        }
    };
}
