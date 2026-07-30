#pragma once

#include <cstdint>

#include "../int_aliases.hpp"
#include "../string/char_util.hpp"

namespace edt
{
constexpr u8 HexToDec(char symbol)
{
    const char s = ToLower(symbol);
    return static_cast<u8>(s >= 'a' ? 10 + s - 'a' : s - '0');
}
}  // namespace edt
