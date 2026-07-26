#pragma once

#include <cstddef>
#include <ranges>

namespace edt
{
inline constexpr auto ArrayIndices2d(size_t size_a, size_t size_b)
{
    return std::views::iota(size_t{0}, size_a) |
           std::views::transform(
               [size_b](size_t ia)
               {
                   return std::views::iota(size_t{0}, size_b) |
                          std::views::transform([ia](size_t ib) { return std::make_tuple(ia, ib); });
               }) |
           std::views::join;
}

}  // namespace edt
