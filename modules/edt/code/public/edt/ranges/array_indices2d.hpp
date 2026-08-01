#pragma once

#include <cstddef>
#include <ranges>
#include <utility>

namespace edt
{
// std::views::cartesian_product expresses this directly but libc++ does not implement it yet.
[[nodiscard]] inline constexpr auto ArrayIndices2d(std::size_t size_a, std::size_t size_b)
{
    return std::views::iota(std::size_t{0}, size_a * size_b) |
           std::views::transform([size_b](std::size_t index)
                                 { return std::pair{index / size_b, index % size_b}; });
}

}  // namespace edt
