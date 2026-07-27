#pragma once

#include <algorithm>
#include <bit>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <span>
#include <type_traits>

#include "edt/branchless/int_if.hpp"

namespace edt
{

// Views an array of unsigned-integer "parts" (words) as one flat bitset. A single
// word is just Extent == 1 - std::span<T, 1> stores only a pointer, so a single-
// word view is the same size and cost as a bare `T*`. Bit ranges are over the
// whole [0, Size()) space; per-word work is an internal detail. Extent may be
// static (std::array / a single word: zero size overhead) or std::dynamic_extent
// (std::vector: the span carries a runtime size, as it must).
template <std::unsigned_integral T, size_t Extent = 1>
class BitsetAdapter
{
public:
    using Mask = std::remove_const_t<T>;
    // Bits per part (word); Size() is the total across all parts.
    static constexpr size_t kBitsCount = sizeof(T) * 8;
    static constexpr Mask kEmptyMask = Mask{};
    static constexpr Mask kFullMask = ~kEmptyMask;
    static constexpr bool kStaticExtent = (Extent != std::dynamic_extent);

public:
    // Single-word view.
    constexpr explicit BitsetAdapter(T& word) noexcept
        requires(Extent == 1)
        : parts_(&word, 1)
    {
    }

    // Multi-word view.
    constexpr explicit BitsetAdapter(std::span<T, Extent> parts) noexcept : parts_(parts) {}

    [[nodiscard]] constexpr size_t PartsCount() const noexcept { return parts_.size(); }

    [[nodiscard]] constexpr size_t Size() const noexcept { return parts_.size() * kBitsCount; }

    constexpr void Set(const size_t index, const bool value) const noexcept
    {
        assert(index < Size());  // NOLINT(misc-static-assert): runtime guard, only constant in the constexpr tests
        SetWordRange(parts_[index / kBitsCount], index % kBitsCount, index % kBitsCount + 1, value);
    }

    [[nodiscard]] constexpr bool Get(const size_t index) const noexcept
    {
        assert(index < Size());  // NOLINT(misc-static-assert): runtime guard, only constant in the constexpr tests
        return (static_cast<Mask>(parts_[index / kBitsCount]) & LeftShifted(Mask{1}, index % kBitsCount)) != kEmptyMask;
    }

    constexpr void SetRange(const size_t begin, const size_t end, const bool value) const noexcept
    {
        if (begin >= end) return;

        const size_t part_begin = begin / kBitsCount;
        const size_t part_end = end / kBitsCount;
        assert(
            part_begin < parts_.size() &&
            part_end <=
                parts_.size());  // NOLINT(misc-static-assert): runtime guard, only constant in the constexpr tests

        const size_t rem_begin = begin % kBitsCount;
        const size_t rem_end = end % kBitsCount;

        if (part_begin == part_end)
        {
            SetWordRange(parts_[part_begin], rem_begin, rem_end, value);
            return;
        }

        // Leading partial word: bits [rem_begin, kBitsCount).
        SetWordRange(parts_[part_begin], rem_begin, kBitsCount, value);

        // Whole middle words: [part_begin + 1, part_end). Address via data() rather
        // than operator[]: part_end equals parts_.size() when the range ends on a
        // word boundary, and forming the one-past-the-end iterator through the
        // bounds-checked span::operator[] is undefined behaviour.
        std::fill(parts_.data() + part_begin + 1, parts_.data() + part_end, IntIf(value, kFullMask, kEmptyMask));

        // Trailing partial word: bits [0, rem_end). Present only when the range does
        // not end on a word boundary - and precisely then part_end is a valid index.
        if (rem_end != 0)
        {
            SetWordRange(parts_[part_end], 0, rem_end, value);
        }
    }

    constexpr void SetN(const size_t begin, const size_t count, const bool value) const noexcept
    {
        SetRange(begin, begin + count, value);
    }

    // Set bits in range [0, count).
    constexpr void SetFirstN(const size_t count, const bool value) const noexcept { SetRange(0, count, value); }

    // Set bits in range [Size() - count, Size()).
    constexpr void SetLastN(const size_t count, const bool value) const noexcept
    {
        SetRange(Size() - count, Size(), value);
    }

    constexpr void Fill(const bool value) const noexcept { SetRange(0, Size(), value); }

    // Applies a raw bit mask to the first word; meaningful for single-word views.
    constexpr void SetMasked(const Mask mask, const bool value) const noexcept
    {
        SetWordMasked(parts_[0], mask, value);
    }

    // Invokes the callback with each set-bit index, lowest first; stops (returning
    // false) as soon as the callback returns false, otherwise returns true.
    template <typename F>
    constexpr bool ForEachBitWithReturn(F&& callback) const
    {
        for (size_t part = 0; part != parts_.size(); ++part)
        {
            Mask word = parts_[part];
            const size_t base = part * kBitsCount;
            while (word != kEmptyMask)
            {
                const auto bit = static_cast<size_t>(std::countr_zero(word));
                if (!callback(base + bit)) return false;
                word &= static_cast<Mask>(word - 1);  // clear the lowest set bit
            }
        }
        return true;
    }

    template <typename F>
    constexpr void ForEachBit(F&& callback) const
    {
        ForEachBitWithReturn(
            [&](const size_t bit_index)
            {
                callback(bit_index);
                return true;
            });
    }

private:
    static constexpr void SetWordMasked(T& word, const Mask mask, const bool value) noexcept
    {
        // Branchless: keep the bits outside the mask, then set the masked bits to
        // `value` (IntIf yields all-ones when value is set, all-zeros otherwise).
        const Mask keep = static_cast<Mask>(word & static_cast<Mask>(~mask));
        const Mask set_bits = static_cast<Mask>(mask & IntIf(value, kFullMask, kEmptyMask));
        word = static_cast<T>(keep | set_bits);
    }

    // Sets bits [begin, end) within one word (0 <= begin <= end <= kBitsCount).
    static constexpr void SetWordRange(T& word, const size_t begin, const size_t end, const bool value) noexcept
    {
        Mask mask = kFullMask;
        RightShift(mask, kBitsCount - (end - begin));
        LeftShift(mask, begin);
        SetWordMasked(word, mask, value);
    }

    // Shifting by >= the operand width is UB, so the shift amount is masked into
    // range (kBitsCount is a power of two) and IntIf then discards the result when
    // the real count would have shifted every bit out. Branchless.
    static constexpr Mask LeftShifted(const Mask mask, const size_t count) noexcept
    {
        const Mask shifted = static_cast<Mask>(mask << (count & (kBitsCount - 1)));
        return IntIf(count < kBitsCount, shifted, kEmptyMask);
    }

    static constexpr void LeftShift(Mask& mask, const size_t count) noexcept { mask = LeftShifted(mask, count); }

    static constexpr Mask RightShifted(const Mask mask, const size_t count) noexcept
    {
        const Mask shifted = static_cast<Mask>(mask >> (count & (kBitsCount - 1)));
        return IntIf(count < kBitsCount, shifted, kEmptyMask);
    }

    static constexpr void RightShift(Mask& mask, const size_t count) noexcept { mask = RightShifted(mask, count); }

private:
    std::span<T, Extent> parts_;
};

// Single word -> Extent 1.
template <std::unsigned_integral T>
BitsetAdapter(T&) -> BitsetAdapter<T, 1>;

// Span (std::array / std::vector view) -> its extent.
template <typename T, size_t E>
BitsetAdapter(std::span<T, E>) -> BitsetAdapter<T, E>;

// Zero-cost abstraction: a static-extent view is exactly a pointer, whatever the
// word count. (A dynamic-extent view necessarily also stores a runtime size.)
static_assert(sizeof(BitsetAdapter<uint64_t>) == sizeof(void*));
static_assert(sizeof(BitsetAdapter<uint64_t, 8>) == sizeof(void*));

}  // namespace edt
