#pragma once

#include "EverydayTools/Geom/Details/MatrixView.h"

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
        template<typename T, size_t, size_t> typename Final
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

        MatrixView<T, 1, nColumns> GetRow(size_t row) {
            MatrixView<T, 1, nColumns> result;
            result.startRow = row;
            result.parentData = GetData();
            result.parentRowSize = nColumns;
            result.rowStride = 1;
            return result;
        }

        MatrixView<std::add_const_t<T>, 1, nColumns> GetRow(size_t row) const {
            MatrixView<std::add_const_t<T>, 1, nColumns> result;
            result.startRow = row;
            result.parentData = GetData();
            result.parentRowSize = nColumns;
            result.rowStride = 1;
            return result;
        }

        MatrixView<T, nRows, 1> GetColumn(size_t column) {
            MatrixView<T, nRows, 1> result;
            result.startColumn = column;
            result.parentData = GetData();
            result.parentRowSize = nColumns;
            result.rowStride = 1;
            return result;
        }

        MatrixView<std::add_const_t<T>, nRows, 1> GetColumn(size_t column) const {
            MatrixView<std::add_const_t<T>, nRows, 1> result;
            result.startColumn = column;
            result.parentData = GetData();
            result.parentRowSize = nColumns;
            result.rowStride = 1;
            return result;
        }

        Table<T, nRows, nColumns> data;
    };
}
