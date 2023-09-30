#pragma once

#include <bit>
#include <cassert>
#include <cstddef>
#include <optional>
#include <type_traits>

namespace edt
{

template <typename T, typename Enable = std::enable_if_t<std::is_unsigned_v<T>>>
class BitIterator
{
public:
    using Mask = std::remove_const_t<T>;
    static constexpr size_t kBitsCount = sizeof(T) * 8;
    static constexpr Mask kEmptyMask = Mask{};
    static constexpr Mask kFullMask = ~kEmptyMask;

public:
    constexpr explicit BitIterator(T& bitset) : bitset_(bitset) {}

    std::optional<size_t> Next()
    {
        assert(bitset_);
        auto shifted = bitset_;
        shifted >>= num_scanned_;
        num_scanned_ += std::countr_zero(shifted);
        if (num_scanned_ < kBitsCount) return num_scanned_++;
        return std::nullopt;
    }

private:
    size_t num_scanned_ = 0;
    T& bitset_ = nullptr;
};

template <typename T>
BitIterator(T&) -> BitIterator<T>;

}  // namespace edt
