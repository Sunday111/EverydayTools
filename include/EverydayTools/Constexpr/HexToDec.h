#pragma once

#include "ToLower.h"

namespace edt
{
    constexpr uint8_t HexToDec(const char symbol) {
        const char s = ToLower(symbol);
        return static_cast<uint8_t>(s >= 'a' ? 10 + s - 'a' : s - '0');
    }
}