#pragma once

#include <bit>
#include <cassert>
#include <cstddef>
#include <type_traits>

#include "BitIterator.hpp"

namespace edt
{

template <typename T, typename Enable = std::enable_if_t<std::is_unsigned_v<T>>>
class BitsetAdapter
{
public:
    using Mask = std::remove_const_t<T>;
    static constexpr size_t kBitsCount = sizeof(T) * 8;
    static constexpr Mask kEmptyMask = Mask{};
    static constexpr Mask kFullMask = ~kEmptyMask;

public:
    constexpr explicit BitsetAdapter(T& bitset) : bitset_(bitset) {}

    constexpr void Set(const size_t index, const bool value) const
    {
        assert(index < kBitsCount);
        Mask mask = 1;
        LeftShift(mask, index);
        SetMasked(mask, value);
    }

    constexpr void SetMasked(const T& mask, const bool value) const
    {
        if (value)
        {
            bitset_ |= mask;
        }
        else
        {
            bitset_ &= ~mask;
        }
    }

    constexpr bool Get(const size_t index) const noexcept
    {
        assert(index < kBitsCount);
        Mask mask = 1;
        LeftShift(mask, index);
        return (bitset_ & mask) != 0;
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

    constexpr void Fill(const bool value) const noexcept
    {
        SetMasked(value ? kFullMask : kEmptyMask);
    }

    size_t NextBitAfter(const size_t ignore_count) const
    {
        Mask copy = bitset_;
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
    static Mask LeftShifted(const Mask mask, const size_t count) noexcept
    {
        if (count < kBitsCount) return mask << count;
        return kEmptyMask;
    }

    static void LeftShift(Mask& mask, size_t count) noexcept
    {
        mask = LeftShifted(mask, count);
    }

    static Mask RightShifted(const Mask mask, const size_t count) noexcept
    {
        if (count < kBitsCount) return mask >> count;
        return kEmptyMask;
    }

    static void RightShift(Mask& mask, size_t count) noexcept
    {
        mask = RightShifted(mask, count);
    }

private:
    T& bitset_;
};

template <typename T>
BitsetAdapter(T&) -> BitsetAdapter<T>;

}  // namespace edt
