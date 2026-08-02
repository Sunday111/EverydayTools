#pragma once

#include <bit>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <optional>
#include <type_traits>

namespace edt
{

template <std::unsigned_integral T>
class BitIterator
{
public:
    using Mask = std::remove_const_t<T>;
    static constexpr std::size_t kBitsCount = sizeof(T) * 8;
    static constexpr Mask kEmptyMask = Mask{};
    static constexpr Mask kFullMask = ~kEmptyMask;

public:
    constexpr explicit BitIterator(T& bitset) : bitset_(&bitset) {}

    std::optional<std::size_t> Next()
    {
        assert(bitset_);

        const Mask remaining = static_cast<Mask>(*bitset_ & static_cast<Mask>(~reported_));
        if (remaining == kEmptyMask) return std::nullopt;

        const auto index = static_cast<std::size_t>(std::countr_zero(remaining));
        const auto bit = static_cast<Mask>(Mask{1} << index);
        reported_ = static_cast<Mask>(bit | static_cast<Mask>(bit - 1));
        return index;
    }

private:
    // Every bit up to and including the one reported last. Masking them off leaves the bits
    // still to visit, so an exhausted iterator simply keeps finding nothing.
    Mask reported_ = kEmptyMask;
    T* bitset_ = nullptr;
};

template <typename T>
BitIterator(T&) -> BitIterator<T>;

}  // namespace edt
