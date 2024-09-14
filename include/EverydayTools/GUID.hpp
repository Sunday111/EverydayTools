#pragma once

#include <array>
#include <cstdint>
#include <ostream>
#include <string_view>

#include "Constexpr/HexToDec.hpp"

namespace edt
{
constexpr uint64_t MakeByte(size_t byteIndex, uint8_t byteValue)
{
    return (static_cast<uint64_t>(byteValue) << (byteIndex * 8));
}

class GUID
{
public:
    friend bool operator==(const GUID& a, const GUID& b) { return a.part1 == b.part1 && a.part2 == b.part2; }

    friend bool operator!=(const GUID& a, const GUID& b) { return !(a == b); }

    bool operator<(const GUID& another) const
    {
        if (part1 == another.part1)
        {
            return part2 < another.part2;
        }

        return part1 < another.part1;
    }

    static constexpr size_t usualStringLength = 36;
    static constexpr size_t stringLengthWithBracers = usualStringLength + 2;

    inline static constexpr GUID Create(const std::string_view& str)
    {
        return str.length() == usualStringLength ? Parse(str) : Parse(str.substr(1, usualStringLength));
    }

    constexpr std::array<char, 36> ToCharArray() const
    {
        std::array<char, 36> result;
        [&]<size_t... indices>(std::index_sequence<indices...>)
        {
            ((result[indices] = GetChar(indices)), ...);
        }(std::make_index_sequence<36>());
        return result;
    };

    constexpr char GetChar(size_t index) const
    {
        constexpr int8_t shifts[]{28, 24, 20, 16, 12, 8,  4,  0,  -1, 44, 40,  36, 32,  -1,  60,  56,  52,  48,
                                  -1, 68, 64, 76, 72, -1, 84, 80, 92, 88, 100, 96, 108, 104, 116, 112, 124, 120};
        int8_t shift = shifts[index];
        if (shift < 0)
        {
            return '-';
        }

        uint64_t v = part1;
        if (shift >= 64)
        {
            v = part2;
            shift %= 64;
        }

        v >>= shift;
        v &= 0xF;

        if (v < 10)
        {
            return '0' + static_cast<char>(v);
        }

        return 'A' - 10 + static_cast<char>(v);
    }

private:
    inline static constexpr GUID Parse(const std::string_view str)
    {
        const auto parsePart = [str](auto indexMap)
        {
            const auto parseBytes = [str, indexMap](const auto... indices)
            {
                const auto parseByte = [str, indexMap](size_t index)
                {
                    const auto parseWord = [str](const size_t byteIndex)
                    {
                        const uint8_t h = HexToDec(str[byteIndex]);
                        const uint8_t l = HexToDec(str[byteIndex + 1]);
                        return static_cast<uint8_t>((h << 4) | l);
                    };

                    return MakeByte(index, parseWord(indexMap[index]));
                };

                return (parseByte(static_cast<size_t>(indices)) | ...);
            };

            return parseBytes(0, 1, 2, 3, 4, 5, 6, 7);
        };

        return GUID{//                              0  1  2  3   4  5  6   7
                    parsePart(std::array<size_t, 8>{6, 4, 2, 0, 11, 9, 16, 14}),
                    parsePart(std::array<size_t, 8>{19, 21, 24, 26, 28, 30, 32, 34})};
    }

public:
    uint64_t part1 = 0;
    uint64_t part2 = 0;
};
}  // namespace edt

namespace std
{
template <>
struct hash<edt::GUID>
{
    [[nodiscard]] inline std::size_t operator()(const edt::GUID& k) const
    {
        std::hash<uint64_t> hasher;
        return hasher(k.part1) ^ hasher(k.part2);
    }
};
}  // namespace std

inline std::ostream& operator<<(std::ostream& stream, const edt::GUID& guid)
{
    const auto char_guid = guid.ToCharArray();
    std::string_view view(char_guid.data(), char_guid.size());
    return stream << view;
}