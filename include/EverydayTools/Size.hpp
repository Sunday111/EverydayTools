#pragma once

#include <cstddef>

namespace edt
{
template <typename T>
inline constexpr std::size_t SizeInBytes = sizeof(T);

template <typename T>
inline constexpr std::size_t SizeInBits = SizeInBytes<T> * 8;
}  // namespace edt
