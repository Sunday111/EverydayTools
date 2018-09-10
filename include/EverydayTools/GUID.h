#pragma once

#include <array>
#include <cstdint>
#include <string_view>

#include "Constexpr/HexToDec.h"

namespace edt
{
    constexpr uint64_t MakeByte(size_t byteIndex, uint8_t byteValue) {
        return (static_cast<uint64_t>(byteValue) << (byteIndex * 8));
    }

    class GUID
    {
    public:
		friend bool operator==(const GUID& a, const GUID& b) {
			return a.part1 == b.part1 && a.part2 == b.part2;
		}

		friend bool operator!=(const GUID& a, const GUID& b) {
			return !(a == b);
		}

		bool operator<(const GUID& another) const {
			if (part1 == another.part1) {
				return part2 < another.part2;
			}

			return part1 < another.part1;
		}

        static constexpr size_t usualStringLength = 36;
        static constexpr size_t stringLengthWithBracers = usualStringLength + 2;
    
        inline static constexpr GUID Create(const std::string_view str) {
            return str.length() == usualStringLength ?
                Parse(str) : Parse(str.substr(1, usualStringLength));
        }
    private:
        inline static constexpr GUID Parse(const std::string_view str) {
            const auto parsePart = [str](auto indexMap) {
                const auto parseBytes = [str, indexMap](const auto... indices) {
                    const auto parseByte = [str, indexMap](size_t index) {
                        const auto parseWord = [str](const size_t byteIndex) {
                            const uint8_t h = HexToDec(str[byteIndex]);
                            const uint8_t l = HexToDec(str[byteIndex + 1]);
                            return static_cast<uint8_t>((h << 4) | l);
                        };
    
                        return MakeByte(index, parseWord(indexMap[index]));
                    };
    
                    return (parseByte(indices) | ...);
                };
    
                return parseBytes(0, 1, 2, 3, 4, 5, 6, 7);
            };
    
            return GUID {
                parsePart(std::array<size_t, 8> { 6, 4, 2, 0, 11, 9, 16, 14 }),
                parsePart(std::array<size_t, 8> { 19, 21, 24, 26, 28, 30, 32, 34 })
            };
        }
    
    public:
        uint64_t part1 = 0;
        uint64_t part2 = 0;
    };
}