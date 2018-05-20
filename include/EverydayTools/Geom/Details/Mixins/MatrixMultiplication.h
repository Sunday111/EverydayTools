#pragma once

#include "EverydayTools/Geom/Details/ImplementCastThis.h"

namespace edt::geom::details::matrix_multiplication
{
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
        EDT_MATRIX_IMPLEMENT_CAST_THIS
    public:
        template
        <
            typename U,
            size_t bColumns,
            template<typename, size_t, size_t> typename Mtx
        >
        decltype(auto) Multiply(const Mtx<U, nColumns, bColumns>& that_) {
            // nRows is n
            // nColumns is m
            // bColumns is p

            //using TR = decltype(std::declval(T) * std::declval(U));
            using TR = T;
            Final<TR, nRows, bColumns> result;

            auto& this_ = CastThis();

            for (size_t i = 0; i < nRows; ++i) {
                for (size_t j = 0; j < bColumns; ++j) {
                    auto this_row = this_.GetRow(i);
                    auto that_column = that_.GetColumn(j);
                    auto dot_prod = this_row.Dot(that_column);
                    result.At(i, j) = dot_prod;
                }
            }

            return result;
        }
    };
}