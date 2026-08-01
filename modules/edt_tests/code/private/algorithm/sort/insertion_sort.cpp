#include "edt/algorithm/sort/insertion_sort.hpp"

#include "gtest/gtest.h"
#include "sort_cases.hpp"

TEST(InsertionSortTest, SortsEveryShapeOfInput)  // NOLINT
{
    sort_cases::CheckAll([](int* data, size_t size) { edt::InsertionSort(data, size); });
}
