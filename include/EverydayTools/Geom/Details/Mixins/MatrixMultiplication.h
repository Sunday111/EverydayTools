#pragma once

#include "EverydayTools/Geom/Details/ImplementCastThis.h"

namespace edt::geom::details::matrix_multiplication
{
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final
    >
    class Mixin
    {
        EDT_MATRIX_IMPLEMENT_CAST_THIS
    public:
        template
        <
            typename U,
            size_t bColumns,
            template<typename, size_t, size_t> typename Mtx
        >
        decltype(auto) Multiply(const Mtx<U, nColumns, bColumns>& arg) {
            // nRows is n
            // nColumns is m
            // bColumns is p

            using TR = decltype(declval(T) * declval(U));
            Final<TR, nRows, bColumns> result;
            return result;
        }
    };
}