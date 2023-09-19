#pragma once

#include <bitset>
#include <cassert>
#include <cstdint>
#include <vector>

namespace edt
{
class Bitset
{
public:
    using Part = uint64_t;
    static constexpr size_t kPartSizeBytes = sizeof(Part);
    static constexpr size_t kPartSizeBits = kPartSizeBytes * 8;
    static constexpr Part kEmptyPart = Part{};
    static constexpr Part kFullPart = ~kEmptyPart;

    Bitset() = default;

    void Resize(const size_t new_size, const bool new_value)
    {
        const size_t new_parts_count = (new_size / kPartSizeBits) + 1;
        parts_.resize(new_parts_count);
        const size_t prev_size = size_;
        size_ = new_size;
        SetRange(prev_size, size_, new_value);
    }

    void Clear()
    {
        parts_.resize(0);
        size_ = 0;
    }

    void Set(const size_t index, const bool value)
    {
        assert(index < size_);
        const size_t part_index = index / kPartSizeBits;
        const size_t index_in_part = index % kPartSizeBits;
        Part mask = 1;
        mask <<= index_in_part;
        SetPartMasked(part_index, mask, value);
    }

    bool Get(const size_t index) const
    {
        assert(index < size_);
        const size_t part_index = index / kPartSizeBits;
        const size_t index_in_part = index % kPartSizeBits;
        Part mask = 1;
        mask <<= index_in_part;
        return (parts_[part_index] & mask) != kEmptyPart;
    }

    void SetRange(const size_t begin, const size_t end, const bool value)
    {
        if (begin >= end) return;

        assert(begin < size_ && end <= size_);

        const size_t part_begin = begin / kPartSizeBits;
        const size_t rem_begin = begin % kPartSizeBits;
        const size_t part_end = end / kPartSizeBits;

        if (part_begin == part_end)
        {
            const size_t count = end - begin;
            Part mask = kFullPart;
            mask >>= kPartSizeBits - count;
            mask <<= rem_begin;
            SetPartMasked(part_begin, mask, value);
        }
        else
        {
            // Patch the first part
            {
                Part mask = kFullPart;
                mask <<= rem_begin;
                SetPartMasked(part_begin, mask, value);
            }

            // fill middle parts
            std::fill(&parts_[part_begin + 1], &parts_[part_end], value ? kFullPart : kEmptyPart);

            // Patch the last part
            {
                const size_t rem_end = end % kPartSizeBits;
                Part mask = kFullPart;
                mask >>= kPartSizeBits - rem_end;
                SetPartMasked(part_end, mask, value);
            }
        }
    }

    size_t Size() const
    {
        return size_;
    }

private:
    void SetPartMasked(const size_t index, const Part& mask, const bool value)
    {
        if (value)
        {
            parts_[index] |= mask;
        }
        else
        {
            parts_[index] &= ~mask;
        }
    }

private:
    std::vector<Part> parts_;
    size_t size_ = 0;
};
}  // namespace edt
