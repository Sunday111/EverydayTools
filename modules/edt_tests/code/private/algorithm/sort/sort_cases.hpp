#pragma once

#include <algorithm>
#include <random>
#include <vector>

#include "gtest/gtest.h"

// The cases every comparison sort here must handle, run against one sort function.
//
// Each case checks two things: the range comes out ordered, and it holds the same
// multiset it started with. std::is_sorted on its own would accept a function that
// zeroed or duplicated elements, so both halves are needed.
namespace sort_cases
{
template <typename Sort>
void ExpectSortedPermutation(Sort&& sort, std::vector<int> values)
{
    std::vector<int> expected = values;
    std::ranges::sort(expected);

    sort(values.empty() ? nullptr : values.data(), values.size());

    EXPECT_EQ(values, expected);
}

template <typename Sort>
void CheckDegenerateAndOrderedInputs(Sort&& sort)
{
    // Size 0 must not dereference the pointer at all.
    sort(static_cast<int*>(nullptr), 0U);

    ExpectSortedPermutation(sort, {});
    ExpectSortedPermutation(sort, {1});
    ExpectSortedPermutation(sort, {1, 2});
    ExpectSortedPermutation(sort, {2, 1});
    ExpectSortedPermutation(sort, {7, 7, 7, 7});
    ExpectSortedPermutation(sort, {1, 2, 3, 4, 5, 6, 7, 8});
    ExpectSortedPermutation(sort, {8, 7, 6, 5, 4, 3, 2, 1});
    ExpectSortedPermutation(sort, {3, 1, 3, 1, 2, 2});
    ExpectSortedPermutation(sort, {0, -1, 5, -100, 100, -1});

    // Extreme values: a comparison sort must not do arithmetic on the elements.
    constexpr int kMin = std::numeric_limits<int>::min();
    constexpr int kMax = std::numeric_limits<int>::max();
    ExpectSortedPermutation(sort, {kMax, kMin, 0, kMax, kMin});
}

// Nothing outside [0, size) may be written, whatever the input ordering.
template <typename Sort>
void CheckStaysInsideTheRange(Sort&& sort)
{
    constexpr int kGuard = 0x5A5A5A;
    for (size_t size = 0; size != 16; ++size)
    {
        std::vector<int> buffer(size + 4, kGuard);
        for (size_t i = 0; i != size; ++i) buffer[i + 2] = static_cast<int>(size - i);

        sort(buffer.data() + 2, size);

        EXPECT_EQ(buffer[0], kGuard) << "size = " << size;
        EXPECT_EQ(buffer[1], kGuard) << "size = " << size;
        EXPECT_EQ(buffer[size + 2], kGuard) << "size = " << size;
        EXPECT_EQ(buffer[size + 3], kGuard) << "size = " << size;
        EXPECT_TRUE(std::is_sorted(buffer.begin() + 2, buffer.begin() + 2 + static_cast<long>(size)))
            << "size = " << size;
    }
}

// Fixed seed: a failure here has to be reproducible.
template <typename Sort>
void CheckAgainstStdSort(Sort&& sort)
{
    std::mt19937 generator(20260730);
    std::uniform_int_distribution<int> value(-50, 50);  // a narrow range, so duplicates are common

    for (size_t size = 0; size != 65; ++size)
    {
        for (size_t attempt = 0; attempt != 8; ++attempt)
        {
            std::vector<int> values(size);
            for (int& v : values) v = value(generator);
            ExpectSortedPermutation(sort, std::move(values));
        }
    }
}

template <typename Sort>
void CheckAll(Sort&& sort)
{
    CheckDegenerateAndOrderedInputs(sort);
    CheckStaysInsideTheRange(sort);
    CheckAgainstStdSort(sort);
}
}  // namespace sort_cases
