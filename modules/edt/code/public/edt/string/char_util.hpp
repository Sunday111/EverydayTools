#pragma once

namespace edt
{

// ASCII-only case conversion, on `char` rather than the `int` of <cctype>.
//
// std::tolower/std::toupper read the current C locale, so they are not usable in
// constant expressions and their result can change under setlocale. They also
// take an int that must be representable as unsigned char: passing a char is
// undefined behaviour for every byte above 0x7F where char is signed. These
// leave anything outside A-Z / a-z untouched, whatever the locale.
[[nodiscard]] constexpr char ToLower(char s)
{
    const bool is_upper_case = (s >= 'A' && s <= 'Z');
    return is_upper_case ? static_cast<char>('a' + s - 'A') : s;
}

[[nodiscard]] constexpr char ToUpper(char s)
{
    const bool is_lower_case = (s >= 'a' && s <= 'z');
    return is_lower_case ? static_cast<char>('A' + s - 'a') : s;
}

}  // namespace edt
