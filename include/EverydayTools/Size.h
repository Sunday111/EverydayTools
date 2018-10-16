#pragma once

template<typename T>
inline constexpr size_t SizeInBytes = sizeof(T);

template<typename T>
inline constexpr size_t SizeInBits = SizeInBytes<T> * 8;