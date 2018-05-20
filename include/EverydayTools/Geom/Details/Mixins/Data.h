#pragma once

#include "EverydayTools/Geom/Details/FwdDecl.h"

namespace edt::geom::details::data {
    template
    <
        typename T,
        size_t N
    >
    using Row = T[N];

    template
    <
        typename T,
        size_t nRows,
        size_t nColumns
    >
    using Table = Row<T, nColumns>[nRows];

    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final,
        template<typename T, size_t, size_t> typename ReturnValue
    >
    class Mixin
    {
    public:
        T& At(size_t nRow, size_t nColumn) noexcept {
            return data[nRow][nColumn];
        }

        const T& At(size_t nRow, size_t nColumn) const noexcept {
            return data[nRow][nColumn];
        }

        T* GetData() {
            return &data[0][0];
        }

        const T* GetData() const {
            return &data[0][0];
        }

        LineMatrixView<T, 1, nColumns> GetRow(size_t row) {
            assert(row < nRows);
            return LineMatrixView<T, 1, nColumns>(&data[row][0]);
        }

        LineMatrixView<std::add_const_t<T>, 1, nColumns> GetRow(size_t row) const {
            assert(row < nRows);
            return LineMatrixView<std::add_const_t<T>, 1, nColumns>(&data[row][0]);
        }

        LineMatrixView<T, nRows, 1> GetColumn(size_t column) {
            assert(column < nColumns);
            return LineMatrixView<T, nRows, 1>(&data[0][column], nColumns);
        }

        LineMatrixView<std::add_const_t<T>, nRows, 1> GetColumn(size_t column) const {
            assert(column < nColumns);
            return LineMatrixView<std::add_const_t<T>, nRows, 1>(&data[0][column], nColumns);
        }

        Table<T, nRows, nColumns> data;
    };
}
