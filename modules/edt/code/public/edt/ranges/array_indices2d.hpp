#pragma once

#include <cstddef>
#include <ranges>
#include <utility>

namespace edt
{
[[nodiscard]] inline constexpr auto ArrayIndices2d(std::size_t size_a, std::size_t size_b)
{
    return std::views::iota(std::size_t{0}, size_a) |
           std::views::transform(
               [size_b](std::size_t ia)
               {
                   return std::views::iota(std::size_t{0}, size_b) |
                          std::views::transform([ia](std::size_t ib) { return std::pair{ia, ib}; });
               }) |
           std::views::join;
}

}  // namespace edt
