#pragma once

#include "../ImplementCastThis.h"

namespace edt::geom::details::not_vector_methods
{
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
    class Mixin<T, nRows, nColumns, Final,
        std::enable_if_t<!is_vector<nRows, nColumns>>>
    {
        EDT_MATRIX_IMPLEMENT_CAST_THIS
    public:
        decltype(auto) operator[](size_t i) {
            return CastThis().GetRow(i);
        }

        decltype(auto) operator[](size_t i) const {
            return CastThis().GetRow(i);
        }
    };
}
