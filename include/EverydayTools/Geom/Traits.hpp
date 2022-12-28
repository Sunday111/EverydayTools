#pragma once

namespace edt::geom
{
    template<size_t Rows, size_t Columns, size_t N>
    constexpr bool is_nd_vector =
        (Rows == 1 && Columns == N) ||
        (Rows == N && Columns == 1);

    template<size_t Rows, size_t Columns>
    constexpr bool is_vector = Rows == 1 || Columns == 1;

    template<size_t Rows, size_t Columns>
    constexpr bool is_square = Rows == Columns;
}
