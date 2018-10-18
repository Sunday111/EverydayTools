#pragma once

#include <type_traits>
#include "TypeFlagTraits.h"

namespace edt::reflection
{
    enum class NumberType
    {
        SignedInteger,
        UnsignedInteger,
        FloatingPoint
    };

    class NumberTypeInfo
    {
    public:
        NumberType numberType;
    };

    class NumberTypeTraits
    {
    public:
        template<typename T>
        static inline constexpr bool IsTypeApplicable = std::is_arithmetic_v<T>;
        using Container = NumberTypeInfo;

        template<typename T>
        static void ConstructContainer(Container& c) {
            if constexpr (std::is_floating_point_v<T>) {
                c.numberType = NumberType::FloatingPoint;
            } else {
                if constexpr (std::is_signed_v<T>) {
                    c.numberType = NumberType::SignedInteger;
                } else {
                    c.numberType = NumberType::UnsignedInteger;
                }
            }
        }
    };

    template<>
    struct TypeFlagToTraitMap<TypeFlags::Number>
    {
        using Trait = NumberTypeTraits;
    };
}
