#pragma once

namespace edt
{
    template<typename T>
    void InsertionSort(T* arr, size_t size) {
        if (size > 1) {
            for (int i = 1; i < size; ++i) {
                for (int j = i; j > 0; --j) {
                    T& a = arr[j];
                    T& b = arr[j - 1];
                    if (a < b) {
                        std::swap(a, b);
                    } else {
                        break;
                    }
                }
            }
        }
    }
}