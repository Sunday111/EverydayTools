#pragma once

#include "EverydayTools/Geom/Details/ImplementCastThis.h"

namespace edt::geom::details::square_matrix {
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final,
        typename Enable = void
    >
    class Mixin
    {

    };

    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final
    >
    class Mixin<T, nRows, nColumns, Final, std::enable_if_t<nRows == nColumns>>
    {
        EDT_MATRIX_IMPLEMENT_CAST_THIS
    public:
        static TFinal Identity() {
            TFinal r;
            for (size_t i = 0; i < nRows; ++i) {
                for (size_t j = 0; j < nColumns; ++j) {
                    r.At(i, j) = i == j ? T(1) : T(0);
                }
            }
            return r;
        }
    };
}
