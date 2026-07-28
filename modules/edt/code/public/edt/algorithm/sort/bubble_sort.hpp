#pragma once

#include <cstddef>
#include <utility>

namespace edt
{
template <typename T>
void BubbleSort(T* arr, std::size_t count)
{
    if (count > 1)
    {
        const std::size_t lastIndex = count - 1;
        for (std::size_t i = 0; i < lastIndex; ++i)
        {
            for (std::size_t j = 0; j < lastIndex - i; ++j)
            {
                T& a = arr[j];
                T& b = arr[j + 1];
                if (b < a)
                {
                    std::swap(b, a);
                }
            }
        }
    }
}
}  // namespace edt
