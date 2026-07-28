#pragma once

#include <cstddef>
#include <utility>

namespace edt
{
template <typename T>
void InsertionSort(T* arr, std::size_t size)
{
    if (size > 1)
    {
        for (std::size_t i = 1; i < size; ++i)
        {
            for (std::size_t j = i; j > 0; --j)
            {
                T& a = arr[j];
                T& b = arr[j - 1];
                if (a < b)
                {
                    std::swap(a, b);
                }
                else
                {
                    break;
                }
            }
        }
    }
}
}  // namespace edt
