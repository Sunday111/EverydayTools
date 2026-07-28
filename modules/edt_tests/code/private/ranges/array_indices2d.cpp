#include "edt/ranges/array_indices2d.hpp"

#include <utility>
#include <vector>

#include "gtest/gtest.h"

TEST(ArrayIndices2dTest, VisitsEveryPairRowMajor)
{
    std::vector<std::pair<std::size_t, std::size_t>> seen;
    for (const auto [a, b] : edt::ArrayIndices2d(3, 2)) seen.emplace_back(a, b);

    const std::vector<std::pair<std::size_t, std::size_t>> expected{{0, 0}, {0, 1}, {1, 0}, {1, 1}, {2, 0}, {2, 1}};
    EXPECT_EQ(seen, expected);
}

TEST(ArrayIndices2dTest, EmptyWhenEitherExtentIsZero)
{
    const auto count = [](auto&& range)
    {
        std::size_t n = 0;
        for ([[maybe_unused]] const auto pair : range) ++n;
        return n;
    };

    EXPECT_EQ(count(edt::ArrayIndices2d(0, 3)), 0U);
    EXPECT_EQ(count(edt::ArrayIndices2d(3, 0)), 0U);
    EXPECT_EQ(count(edt::ArrayIndices2d(4, 5)), 20U);
}
