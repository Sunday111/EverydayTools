#include "edt/algorithm/sort/bubble_sort.hpp"

#include "gtest/gtest.h"
#include "sort_cases.hpp"

TEST(BubbleSortTest, SortsEveryShapeOfInput)  // NOLINT
{
    sort_cases::CheckAll([](int* data, size_t size) { edt::BubbleSort(data, size); });
}
