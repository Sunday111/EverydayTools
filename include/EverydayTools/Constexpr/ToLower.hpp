#pragma once

namespace edt
{
    constexpr char ToLower(const char s) {
        const bool isBigLetter = (s >= 'A' && s <= 'Z');
        return isBigLetter ? 'a' + s - 'A' : s;
    }
}