#pragma once

#include "ThrowIfFailed.h"
#include <numeric>
#include <type_traits>

namespace edt
{
    // Allows to find numeric overflows
    template<typename To, typename From>
    To CheckedCast(const From& from) {
        constexpr auto exceptionMessage = "Type overflow";
        using Limits = std::numeric_limits<To>;
        if constexpr (
            (std::is_signed_v<From> && std::is_unsigned_v<To>) ||
            (std::is_signed_v<To> && std::is_unsigned_v<From>)) {
            ThrowIfFailed(from >= static_cast<From>(0), exceptionMessage);
            ThrowIfFailed(from <= Limits::max(), exceptionMessage);
        } else {
            ThrowIfFailed(from >= Limits::lowest(), exceptionMessage);
            ThrowIfFailed(from <= Limits::max(), exceptionMessage);
        }
        return static_cast<To>(from);
    }
}