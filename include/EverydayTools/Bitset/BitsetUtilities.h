#pragma once

#include "EnumBitset.h"
#include "../Size.h"
#include "../Template/TypesList.h"
#include "../Template/ValuesList.h"

namespace edt
{
    template<auto bitset>
    struct FlagIsSetFilter
    {
        template<auto flag>
        struct Functor
        {
            static constexpr bool Value = FlagIsSet(bitset, flag);
        };
    };

    template<typename T>
    struct MakeFlagByIndexMapFunctor
    {
        template<auto index>
        struct Functor
        {
            static constexpr auto Value = MakeFlagByIndex<T, index>();
        };
    };

    template<typename T>
    constexpr T MakeFlagByIndex(const size_t index) {
        return (T(1) << index);
    }

    template<typename T, size_t index>
    constexpr T MakeFlagByIndex() {
        static_assert(index < SizeInBits<T>);
        return MakeFlagByIndex<T>(index);
    }

    template<typename T>
    constexpr bool FlagIsSet(const T flags, const T flag) {
        if constexpr (std::is_enum_v<T> && edt::enable_enum_bitset_v<T>) {
            using U = std::underlying_type_t<T>;
            return (static_cast<U>(flags) & static_cast<U>(flag)) != U{ 0 };
        } else {
            return (flags & flag) != T{ 0 };
        }
    }

    template<typename T>
    constexpr size_t GetSetFlagsCount(const T flags) {
        size_t r = 0;
        for (size_t i = 0; i < SizeInBits<T>; ++i) {
            const T flagAtIndex = MakeFlagByIndex<T>(i);
            if (FlagIsSet(flags, flagAtIndex)) {
                ++r;
            }
        }

        return r;
    }

    template<auto... values>
    constexpr auto FlagsToBitset(edt::ValuesList<values...>) {
        return (values | ...);
    }
}