#pragma once

#include <cassert>
#include <cstdint>
#include <vector>

#include "BitsetArrayAdapter.hpp"

namespace edt
{
class DynamicBitset
{
public:
    using Part = uint64_t;
    static constexpr size_t kPartSizeBytes = sizeof(Part);
    static constexpr size_t kPartSizeBits = kPartSizeBytes * 8;
    static constexpr Part kEmptyPart = Part{};
    static constexpr Part kFullPart = ~kEmptyPart;

    DynamicBitset() = default;

    void Resize(const size_t new_size, const bool new_value)
    {
        size_t new_parts_count = new_size / kPartSizeBits;
        if (new_size % kPartSizeBits) ++new_parts_count;
        parts_.resize(new_parts_count);
        const size_t prev_size = size_;
        size_ = new_size;
        if (size_ > prev_size)
        {
            SetRange(prev_size, size_, new_value);
        }
    }

    void Clear()
    {
        parts_.resize(0);
        size_ = 0;
    }

    void Set(const size_t index, const bool value)
    {
        assert(index < size_);
        Adapter().Set(index, value);
    }

    bool Get(const size_t index) const
    {
        assert(index < size_);
        return Adapter().Get(index);
    }

    void SetRange(const size_t begin, const size_t end, const bool value)
    {
        assert(begin <= end && begin < size_ && end <= size_);
        Adapter().SetRange(begin, end, value);
    }

    size_t Size() const { return size_; }

private:
    BitsetArrayAdapter<Part> Adapter() { return BitsetArrayAdapter(std::span(parts_)); }

    BitsetArrayAdapter<const Part> Adapter() const { return BitsetArrayAdapter(std::span(parts_)); }

private:
    std::vector<Part> parts_;
    size_t size_ = 0;
};
}  // namespace edt
