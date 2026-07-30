#pragma once

#include <algorithm>
#include <array>
#include <compare>
#include <cstddef>
#include <string_view>

namespace edt
{
template <std::size_t N>
struct StringLiteral
{
    constexpr StringLiteral(const char (&str)[N])  // NOLINT(google-explicit-constructor,modernize-avoid-c-arrays)
    {
        std::ranges::copy(str, value.begin());
    }

    [[nodiscard]] constexpr std::string_view GetView() const { return {value.data(), N - 1}; }

    [[nodiscard]] constexpr std::size_t Size() const { return N - 1; }

    [[nodiscard]] friend constexpr bool operator==(const StringLiteral&, const StringLiteral&) = default;
    [[nodiscard]] friend constexpr auto operator<=>(const StringLiteral&, const StringLiteral&) = default;

    std::array<char, N> value{};
};
}  // namespace edt
