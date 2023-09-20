#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <tuple>

#include "BitsetAdapter.hpp"
#include "BitsetArrayAdapter.hpp"

namespace edt::fixed_bitset_internals
{
using AllParts = std::tuple<uint64_t, uint32_t, uint16_t, uint8_t>;

inline constexpr size_t BiggestPartTypeIndexWithNoWaste(const size_t bytes_required)
{
    constexpr size_t count = std::tuple_size_v<AllParts>;
    std::array<size_t, count> values{};
    auto get_wasted_space = [&]<typename Part>(std::tuple<Part>)
    {
        size_t parts_count = bytes_required / sizeof(Part);
        if (bytes_required % sizeof(Part)) ++parts_count;
        return parts_count * sizeof(Part) - bytes_required;
    };
    [&]<size_t... type_index>(std::index_sequence<type_index...>)
    {
        ((values[type_index] = get_wasted_space(std::tuple<std::tuple_element_t<type_index, AllParts>>{})), ...);
    }
    (std::make_index_sequence<count>());

    size_t min_index = 0;
    for (size_t i = 1; i != values.size(); ++i)
    {
        if (values[i] < values[min_index])
        {
            min_index = i;
        }
    }

    return min_index;
}

template <size_t bytes_required>
using BiggestPartTypeWithNoWaste_Bytes =
    std::tuple_element_t<BiggestPartTypeIndexWithNoWaste(bytes_required), AllParts>;

static_assert(std::is_same_v<BiggestPartTypeWithNoWaste_Bytes<7>, uint8_t>);
static_assert(std::is_same_v<BiggestPartTypeWithNoWaste_Bytes<2>, uint16_t>);
static_assert(std::is_same_v<BiggestPartTypeWithNoWaste_Bytes<4>, uint32_t>);
static_assert(std::is_same_v<BiggestPartTypeWithNoWaste_Bytes<8>, uint64_t>);

inline constexpr size_t BytesRequiredForBitset(const size_t bits_count)
{
    size_t result = bits_count / 8;
    if (bits_count % 8) ++result;
    return result;
}

template <size_t bits_required>
using BiggestPartTypeWithNoWaste = BiggestPartTypeWithNoWaste_Bytes<BytesRequiredForBitset(bits_required)>;

template <typename Part>
inline constexpr size_t PartsCount(const size_t bits_count)
{
    constexpr size_t part_size_bits = sizeof(Part) * 8;
    size_t parts_count = bits_count / part_size_bits;
    if (bits_count % part_size_bits) ++parts_count;
    return parts_count;
}

}  // namespace edt::fixed_bitset_internals

namespace edt
{

template <size_t bits_count, typename Part_>
class FixedBitsetEx
{
public:
    using Part = Part_;

    static constexpr size_t Size() noexcept
    {
        return bits_count;
    }

    static constexpr size_t PartsCount() noexcept
    {
        return fixed_bitset_internals::PartsCount<Part>(Size());
    }

    inline constexpr void Set(const size_t index, const bool value) noexcept
    {
        assert(index < Size());
        Adapter().Set(index, value);
    }

    inline constexpr bool Get(const size_t index) const noexcept
    {
        assert(index < Size());
        return Adapter().Get(index);
    }

private:
    constexpr auto Adapter() noexcept
    {
        return BitsetArrayAdapter(std::span(parts_));
    }

    constexpr auto Adapter() const noexcept
    {
        return BitsetArrayAdapter(std::span(parts_));
    }

private:
    std::array<Part, PartsCount()> parts_{};
};

template <size_t bits_count>
using FixedBitset = FixedBitsetEx<bits_count, fixed_bitset_internals::BiggestPartTypeWithNoWaste<bits_count>>;
}  // namespace edt
