#pragma once

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

        Table<T, nRows, nColumns> data;
    };
}
