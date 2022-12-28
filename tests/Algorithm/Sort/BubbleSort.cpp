#include "EverydayTools/Algorithm/Sort/BubbleSort.hpp"

#include <array>

#include "EverydayTools/Algorithm/Sort/InsertionSort.hpp"
#include "TestTools.hpp"
#include "gtest/gtest.h"

template <typename T, typename V>
void DoSortTest(T& arr, void (*fn)(V*, size_t)) {
  fn(&arr[0], arr.size());
  const bool ok = std::is_sorted(arr.begin(), arr.end());
  EXPECT_TRUE(ok);
}

template <typename T>
void DoSortTestCases(void (*fn)(T*, size_t)) {
  // One element test
  {
    std::array<int, 1> arr{1};
    DoSortTest(arr, fn);
  }

  // Two elements test
  {
    std::array<int, 2> arr{2, 1};
    DoSortTest(arr, fn);
  }

  // Regular test
  {
    auto arr = GenerateRandomArray(3, 1000, 0, 1000);
    DoSortTest(arr, fn);
  }
}

TEST(AlgorithmTest, Sort) {
  DoSortTestCases<int>(edt::BubbleSort);
  DoSortTestCases<int>(edt::InsertionSort);
}
