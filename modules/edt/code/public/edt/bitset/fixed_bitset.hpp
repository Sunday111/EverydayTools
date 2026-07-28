#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <span>
#include <tuple>

#include "../int_aliases.hpp"
#include "bitset_adapter.hpp"

namespace edt::fixed_bitset_internals
{
using AllParts = std::tuple<u64, u32, u16, u8>;

inline constexpr std::size_t BiggestPartTypeIndexWithNoWaste(std::size_t bytes_required)
{
    constexpr std::size_t count = std::tuple_size_v<AllParts>;
    std::array<std::size_t, count> values{};
    auto get_wasted_space = [&]<typename Part>(std::tuple<Part>)
    {
        std::size_t parts_count = bytes_required / sizeof(Part);
        if (bytes_required % sizeof(Part)) ++parts_count;
        return parts_count * sizeof(Part) - bytes_required;
    };
    [&]<std::size_t... type_index>(std::index_sequence<type_index...>)
    {
        ((values[type_index] = get_wasted_space(std::tuple<std::tuple_element_t<type_index, AllParts>>{})), ...);
    }(std::make_index_sequence<count>());

    std::size_t min_index = 0;
    for (std::size_t i = 1; i != values.size(); ++i)
    {
        if (values[i] < values[min_index])
        {
            min_index = i;
        }
    }

    return min_index;
}

template <std::size_t bytes_required>
using BiggestPartTypeWithNoWaste_Bytes =
    std::tuple_element_t<BiggestPartTypeIndexWithNoWaste(bytes_required), AllParts>;

static_assert(std::is_same_v<BiggestPartTypeWithNoWaste_Bytes<7>, u8>);
static_assert(std::is_same_v<BiggestPartTypeWithNoWaste_Bytes<2>, u16>);
static_assert(std::is_same_v<BiggestPartTypeWithNoWaste_Bytes<4>, u32>);
static_assert(std::is_same_v<BiggestPartTypeWithNoWaste_Bytes<8>, u64>);

inline constexpr std::size_t BytesRequiredForBitset(std::size_t bits_count)
{
    std::size_t result = bits_count / 8;
    if (bits_count % 8) ++result;
    return result;
}

template <std::size_t bits_required>
using BiggestPartTypeWithNoWaste = BiggestPartTypeWithNoWaste_Bytes<BytesRequiredForBitset(bits_required)>;

template <typename Part>
inline constexpr std::size_t PartsCount(std::size_t bits_count)
{
    constexpr std::size_t part_size_bits = sizeof(Part) * 8;
    std::size_t parts_count = bits_count / part_size_bits;
    if (bits_count % part_size_bits) ++parts_count;
    return parts_count;
}

}  // namespace edt::fixed_bitset_internals

namespace edt
{

template <std::size_t bits_count, typename Part_>
class FixedBitsetEx
{
public:
    using Part = Part_;

    static constexpr std::size_t Size() noexcept { return bits_count; }

    static constexpr std::size_t PartsCount() noexcept { return fixed_bitset_internals::PartsCount<Part>(Size()); }

    inline constexpr void Set(std::size_t index, bool value) noexcept
    {
        assert(index < Size());
        Adapter().Set(index, value);
    }

    inline constexpr bool Get(std::size_t index) const noexcept
    {
        assert(index < Size());
        return Adapter().Get(index);
    }

    constexpr void SetRange(std::size_t begin, std::size_t end, bool value) noexcept
    {
        assert(end <= Size());
        Adapter().SetRange(begin, end, value);
    }

    constexpr void Fill(bool value) noexcept { Adapter().SetRange(0, Size(), value); }

private:
    constexpr auto Adapter() noexcept { return BitsetAdapter(std::span(parts_)); }

    constexpr auto Adapter() const noexcept { return BitsetAdapter(std::span(parts_)); }

private:
    std::array<Part, PartsCount()> parts_{};
};

template <std::size_t bits_count>
using FixedBitset = FixedBitsetEx<bits_count, fixed_bitset_internals::BiggestPartTypeWithNoWaste<bits_count>>;
}  // namespace edt
