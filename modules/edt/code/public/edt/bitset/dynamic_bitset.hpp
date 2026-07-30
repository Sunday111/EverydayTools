#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

#include "../int_aliases.hpp"
#include "bitset_adapter.hpp"

namespace edt
{
class DynamicBitset
{
public:
    using Part = u64;
    static constexpr std::size_t kPartSizeBytes = sizeof(Part);
    static constexpr std::size_t kPartSizeBits = kPartSizeBytes * 8;
    static constexpr Part kEmptyPart = Part{};
    static constexpr Part kFullPart = ~kEmptyPart;

    DynamicBitset() = default;

    void Resize(std::size_t new_size, bool new_value)
    {
        std::size_t new_parts_count = new_size / kPartSizeBits;
        if (new_size % kPartSizeBits) ++new_parts_count;
        parts_.resize(new_parts_count);
        const std::size_t prev_size = size_;
        size_ = new_size;
        if (size_ > prev_size)
        {
            SetRange(prev_size, size_, new_value);
        }
    }

    void Clear()
    {
        parts_.clear();
        size_ = 0;
    }

    void Set(std::size_t index, bool value)
    {
        assert(index < size_);
        Adapter().Set(index, value);
    }

    bool Get(std::size_t index) const
    {
        assert(index < size_);
        return Adapter().Get(index);
    }

    void SetRange(std::size_t begin, std::size_t end, bool value)
    {
        assert(begin <= end && begin < size_ && end <= size_);
        Adapter().SetRange(begin, end, value);
    }

    std::size_t Size() const { return size_; }

private:
    BitsetAdapter<Part, std::dynamic_extent> Adapter() { return BitsetAdapter(std::span(parts_)); }

    BitsetAdapter<const Part, std::dynamic_extent> Adapter() const { return BitsetAdapter(std::span(parts_)); }

private:
    std::vector<Part> parts_;
    std::size_t size_ = 0;
};
}  // namespace edt
