#pragma once

#include <array>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <format>
#include <functional>
#include <ostream>
#include <string_view>

#include "constexpr/hex_to_dec.hpp"
#include "int_aliases.hpp"

namespace edt
{
constexpr u64 MakeByte(std::size_t byte_index, u8 byte_value)
{
    return (static_cast<u64>(byte_value) << (byte_index * 8));
}

class GUID
{
public:
    [[nodiscard]] friend constexpr bool operator==(const GUID&, const GUID&) = default;
    [[nodiscard]] friend constexpr std::strong_ordering operator<=>(const GUID&, const GUID&) = default;

    static constexpr std::size_t usualStringLength = 36;
    static constexpr std::size_t stringLengthWithBracers = usualStringLength + 2;

    [[nodiscard]] static constexpr GUID Create(std::string_view str)
    {
        return str.length() == usualStringLength ? Parse(str) : Parse(str.substr(1, usualStringLength));
    }

    [[nodiscard]] constexpr std::array<char, usualStringLength> ToCharArray() const
    {
        std::array<char, usualStringLength> result{};
        [&]<std::size_t... indices>(std::index_sequence<indices...>)
        {
            ((result[indices] = GetChar(indices)), ...);
        }(std::make_index_sequence<usualStringLength>());
        return result;
    }

    [[nodiscard]] constexpr char GetChar(std::size_t index) const
    {
        constexpr std::array<i8, usualStringLength> shifts{28, 24, 20, 16, 12,  8,  4,   0,   -1,  44,  40,  36,
                                                           32, -1, 60, 56, 52,  48, -1,  68,  64,  76,  72,  -1,
                                                           84, 80, 92, 88, 100, 96, 108, 104, 116, 112, 124, 120};
        i8 shift = shifts[index];
        if (shift < 0)
        {
            return '-';
        }

        u64 v = part1;
        if (shift >= 64)
        {
            v = part2;
            shift %= 64;
        }

        v >>= shift;
        v &= 0xF;

        if (v < 10)
        {
            return static_cast<char>('0' + static_cast<char>(v));
        }

        return static_cast<char>('A' - 10 + static_cast<char>(v));
    }

private:
    [[nodiscard]] static constexpr GUID Parse(std::string_view str)
    {
        const auto parsePart = [str](auto indexMap)
        {
            const auto parseBytes = [str, indexMap](const auto... indices)
            {
                const auto parseByte = [str, indexMap](std::size_t index)
                {
                    const auto parseWord = [str](std::size_t byteIndex)
                    {
                        const u8 h = HexToDec(str[byteIndex]);
                        const u8 l = HexToDec(str[byteIndex + 1]);
                        return static_cast<u8>((h << 4) | l);
                    };

                    return MakeByte(index, parseWord(indexMap[index]));
                };

                return (parseByte(static_cast<std::size_t>(indices)) | ...);
            };

            return parseBytes(0, 1, 2, 3, 4, 5, 6, 7);
        };

        return GUID{
            //                                               0  1  2  3   4  5  6   7
            .part1 = parsePart(std::array<std::size_t, 8>{6, 4, 2, 0, 11, 9, 16, 14}),
            .part2 = parsePart(std::array<std::size_t, 8>{19, 21, 24, 26, 28, 30, 32, 34})};
    }

public:
    u64 part1 = 0;
    u64 part2 = 0;
};

inline std::ostream& operator<<(std::ostream& stream, const GUID& guid)
{
    const auto chars = guid.ToCharArray();
    return stream << std::string_view(chars.data(), chars.size());
}
}  // namespace edt

template <>
struct std::hash<edt::GUID>
{
    [[nodiscard]] std::size_t operator()(const edt::GUID& k) const noexcept
    {
        const std::size_t h1 = std::hash<edt::u64>{}(k.part1);
        const std::size_t h2 = std::hash<edt::u64>{}(k.part2);
        return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
    }
};

template <>
struct std::formatter<edt::GUID> : std::formatter<std::string_view>
{
    template <typename FormatContext>
    auto format(const edt::GUID& guid, FormatContext& ctx) const
    {
        const auto chars = guid.ToCharArray();
        return std::formatter<std::string_view>::format(std::string_view(chars.data(), chars.size()), ctx);
    }
};
