#pragma once

#include <cassert>
#include "EverydayTools/Geom/Details/ImplementCastThis.h"

namespace edt::geom::details::line_data_view
{
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final,
        template<typename T, size_t, size_t> typename ReturnValue,
        typename Enable = void
    >
    class Mixin
    {
    };

    // Specialization for row view
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final,
        template<typename T, size_t, size_t> typename ReturnValue
    >
    class Mixin<T, nRows, nColumns, Final, ReturnValue, std::enable_if_t<nRows == 1>>
    {
        EDT_MATRIX_IMPLEMENT_CAST_THIS
    public:
        Mixin(T* data) :
            m_data(data)
        {
        }

        T & At(size_t nRow, size_t nColumn) noexcept {
            return const_cast<T&>(const_cast<const Mixin*>(this)->At(nRow, nColumn));
        }

        const T& At(size_t nRow, size_t nColumn) const noexcept {
            assert(nRow == 0);
            assert(m_data != nullptr);
            assert(nColumn < nColumns);
            return m_data[nColumn];
        }

        T* GetData() {
            return m_data;
        }

        const T* GetData() const {
            return m_data;
        }

        Mixin GetRow(size_t row) {
            assert(row < nRows);
            return *this;
        }

        Final<T, 1, 1> GetColumn(size_t column) {
            assert(column < nColumns);
            return Final<T, 1, 1>(&At(0, column));
        }

    private:
        T* m_data = nullptr;
    };

    // Specialization for column view
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final,
        template<typename T, size_t, size_t> typename ReturnValue
    >
    class Mixin<T, nRows, nColumns, Final, ReturnValue, std::enable_if_t<(nColumns == 1) &&(nRows > 1)>>
    {
        EDT_MATRIX_IMPLEMENT_CAST_THIS
    public:
        Mixin(T* data, size_t stride) :
            m_data(data),
            m_stride(stride)
        {
        }

        T & At(size_t nRow, size_t nColumn) noexcept {
            return const_cast<T&>(const_cast<const Mixin*>(this)->At(nRow, nColumn));
        }

        const T& At(size_t nRow, size_t nColumn) const noexcept {
            assert(m_data != nullptr);
            assert(nColumn == 0);
            assert(nRow < nRows);
            return m_data[nRow * m_stride];
        }

        T* GetData() {
            return m_data;
        }

        const T* GetData() const {
            return m_data;
        }

        Final<T, 1, 1> GetRow(size_t row) {
            assert(row < nRows);
            return Final<T, 1, 1>(&At(row, 0));
        }

        Mixin GetColumn(size_t column) {
            assert(column < nColumns);
            return *this;
        }

    private:
        T* m_data = nullptr;
        size_t m_stride = 1;
    };
}