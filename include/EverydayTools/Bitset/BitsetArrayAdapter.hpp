#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <span>
#include <type_traits>

#include "BitsetAdapter.hpp"

namespace edt
{

template <typename T, size_t Extent = std::dynamic_extent, typename Enable = std::enable_if_t<std::is_unsigned_v<T>>>
class BitsetArrayAdapter
{
public:
    using Mask = std::remove_const_t<T>;
    static constexpr size_t kPartSizeBits = sizeof(T) * 8;
    static constexpr Mask kEmptyMask = Mask{};
    static constexpr Mask kFullMask = ~kEmptyMask;

public:
    explicit BitsetArrayAdapter(std::span<T, Extent> parts) : parts_(parts) {}

    constexpr void SetRange(const size_t begin, const size_t end, const bool value) const noexcept
    {
        if (begin >= end) return;

        const size_t part_begin = begin / kPartSizeBits;
        const size_t part_end = end / kPartSizeBits;
        assert(part_begin < parts_.size() && part_end <= parts_.size());

        const size_t rem_begin = begin % kPartSizeBits;
        const size_t rem_end = end % kPartSizeBits;

        if (part_begin == part_end)
        {
            Part(part_begin).SetRange(rem_begin, rem_end, value);
        }
        else
        {
            // Patch the first part
            Part(part_begin).SetLastN(kPartSizeBits - rem_begin, value);

            // fill middle parts
            std::fill(&parts_[part_begin + 1], &parts_[part_end], value ? kFullMask : kEmptyMask);

            // Patch the last part
            if (end < parts_.size())
            {
                Part(part_begin).SetFirstN(rem_end, value);
            }
        }
    }

    void Set(const size_t index, const bool value)
    {
        const size_t part_index = index / kPartSizeBits;
        const size_t index_in_part = index % kPartSizeBits;
        Part(part_index).Set(index_in_part, value);
    }

    bool Get(const size_t index) const
    {
        const size_t part_index = index / kPartSizeBits;
        const size_t index_in_part = index % kPartSizeBits;
        return Part(part_index).Get(index_in_part);
    }

    constexpr BitsetAdapter<T> Part(const size_t part_index) const noexcept
    {
        return BitsetAdapter{parts_[part_index]};
    }

private:
    std::span<T, Extent> parts_;
};

template <typename T>
BitsetArrayAdapter(T) -> BitsetArrayAdapter<typename T::element_type, T::extent>;

}  // namespace edt
