#pragma once

#include <algorithm>

namespace edt
{
    template<typename T>
    void BubbleSort(T* arr, size_t count) {
        if(count > 1) {
            const size_t lastIndex = count - 1;
            for(size_t i = 0; i < lastIndex; ++i) {
                for (size_t j = 0; j < lastIndex - i; ++j) {
                    T& a = arr[j];
                    T& b = arr[j + 1];
                    if(b < a) {
                        std::swap(b, a);
                    }
                }
            }
        }
    }
}