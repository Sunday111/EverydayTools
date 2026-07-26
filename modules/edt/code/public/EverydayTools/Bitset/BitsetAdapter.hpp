#pragma once

#include <bit>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <type_traits>

#include "EverydayTools/Branchless/IntIf.hpp"

namespace edt
{

template <std::unsigned_integral T>
class BitsetAdapter
{
public:
    using Mask = std::remove_const_t<T>;
    static constexpr size_t kBitsCount = sizeof(T) * 8;
    static constexpr Mask kEmptyMask = Mask{};
    static constexpr Mask kFullMask = ~kEmptyMask;

public:
    constexpr explicit BitsetAdapter(T& bitset) : bitset_(&bitset) {}

    constexpr void Set(const size_t index, const bool value) const
    {
        assert(index < kBitsCount);
        Mask mask = 1;
        LeftShift(mask, index);
        SetMasked(mask, value);
    }

    constexpr void SetMasked(const Mask mask, const bool value) const noexcept
    {
        // Branchless: keep the bits outside the mask, then set the masked bits to
        // `value` (IntIf yields all-ones when value is set, all-zeros otherwise).
        const Mask keep = static_cast<Mask>(*bitset_ & static_cast<Mask>(~mask));
        const Mask set_bits = static_cast<Mask>(mask & IntIf(value, kFullMask, kEmptyMask));
        *bitset_ = static_cast<T>(keep | set_bits);
    }

    [[nodiscard]] constexpr bool Get(const size_t index) const noexcept
    {
        assert(index < kBitsCount);
        Mask mask = 1;
        LeftShift(mask, index);
        return (*bitset_ & mask) != 0;
    }

    constexpr void SetN(const size_t begin, const size_t count, const bool value) const noexcept
    {
        assert(begin < kBitsCount && (begin + count) <= kBitsCount);
        Mask mask = kFullMask;
        RightShift(mask, kBitsCount - count);
        LeftShift(mask, begin);
        SetMasked(mask, value);
    }

    constexpr void SetRange(const size_t begin, const size_t end, const bool value) const noexcept
    {
        assert(begin < kBitsCount && end <= kBitsCount);
        const size_t count = end - begin;
        Mask mask = kFullMask;
        RightShift(mask, kBitsCount - count);
        LeftShift(mask, begin);
        SetMasked(mask, value);
    }

    // Set bits in range [kBitsCount - count, kBitsCount)
    constexpr void SetLastN(const size_t count, const bool value) const noexcept
    {
        Mask mask = kFullMask;
        LeftShift(mask, kBitsCount - count);
        SetMasked(mask, value);
    }

    // Set bits in range [0, count)
    constexpr void SetFirstN(const size_t count, const bool value) const noexcept
    {
        Mask mask = kFullMask;
        RightShift(mask, kBitsCount - count);
        SetMasked(mask, value);
    }

    constexpr void Fill(const bool value) const noexcept { SetMasked(kFullMask, value); }

    [[nodiscard]] size_t NextBitAfter(const size_t ignore_count) const
    {
        Mask copy = *bitset_;
        copy &= LeftShifted(kFullMask, ignore_count);
        return std::countr_zero(copy);
    }

    // Invokes callback with each bit index starting from the lowest
    template <typename F>
    bool ForEachBitWithReturn(F&& callback) const
    {
        size_t num_scanned = 0;
        while (true)
        {
            num_scanned = NextBitAfter(num_scanned);
            if (num_scanned >= kBitsCount)
            {
                return true;
            }
            if (!callback(num_scanned++))
            {
                return false;
            }
        }
    }

    // Invokes callback with each bit index starting from the lowest
    template <typename F>
    void ForEachBit(F&& callback) const
    {
        ForEachBitWithReturn(
            [&](const size_t bit_index)
            {
                callback(bit_index);
                return true;
            });
    }

private:
    // Shifting by >= the operand width is UB, so the shift amount is masked into
    // range (kBitsCount is a power of two) and IntIf then discards the result
    // when the real count would have shifted every bit out. Branchless.
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
    T* bitset_ = nullptr;
};

template <typename T>
BitsetAdapter(T&) -> BitsetAdapter<T>;

}  // namespace edt
