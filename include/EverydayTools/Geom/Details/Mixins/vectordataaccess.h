#pragma once

#include "EverydayTools/Geom/Details/ImplementCastThis.h"
#include "EverydayTools/Geom/Details/IsNdVector.h"
#include "EverydayTools/Preprocessor/EmptyBases.h"

namespace edt::geom::details::vector_data_access
{
#define EDT_IMPLEMENT_FIELD_MIXIN($name, $index)                                        \
    template                                                                            \
    <                                                                                   \
        typename T,                                                                     \
        size_t nRows,                                                                   \
        size_t nColumns,                                                                \
        template<typename T, size_t, size_t> typename Final                             \
    >                                                                                   \
    class $name##_mixin                                                                 \
    {                                                                                   \
        EDT_MATRIX_IMPLEMENT_CAST_THIS                                                  \
        static_assert($index < (nRows > nColumns ? nRows : nColumns), "Invalid usage"); \
    public:                                                                             \
        T $name() const noexcept { return CastThis().Elem($index); }                    \
        T& r##$name() noexcept { return CastThis().Elem($index); }                      \
        const T& r##$name() const noexcept { return CastThis().Elem($index); }          \
    }

    // X Y Z W mixins
    EDT_IMPLEMENT_FIELD_MIXIN(x, 0);
    EDT_IMPLEMENT_FIELD_MIXIN(y, 1);
    EDT_IMPLEMENT_FIELD_MIXIN(z, 2);
    EDT_IMPLEMENT_FIELD_MIXIN(w, 3);

    // I J K mixins
    EDT_IMPLEMENT_FIELD_MIXIN(i, 0);
    EDT_IMPLEMENT_FIELD_MIXIN(j, 1);
    EDT_IMPLEMENT_FIELD_MIXIN(k, 2);

    // R G B A mixins
    EDT_IMPLEMENT_FIELD_MIXIN(r, 0);
    EDT_IMPLEMENT_FIELD_MIXIN(g, 1);
    EDT_IMPLEMENT_FIELD_MIXIN(b, 2);
    EDT_IMPLEMENT_FIELD_MIXIN(a, 3);

    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final,
        typename Enable = void
    >
    class EDT_EMPTY_BASES Mixin
    {
    };

    // Acessors for two dimensional vectors
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final
    >
    class EDT_EMPTY_BASES Mixin<T, nRows, nColumns, Final,
        std::enable_if_t<is_nd_vector<nRows, nColumns, 2>>> :
        // X Y
        public x_mixin<T, nRows, nColumns, Final>,
        public y_mixin<T, nRows, nColumns, Final>,
        // I J
        public i_mixin<T, nRows, nColumns, Final>,
        public j_mixin<T, nRows, nColumns, Final>
    {
    };

    // Acessors for three dimensional vectors
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final
    >
    class EDT_EMPTY_BASES Mixin<T, nRows, nColumns, Final,
        std::enable_if_t<is_nd_vector<nRows, nColumns, 3>>> :
        // X Y Z
        public x_mixin<T, nRows, nColumns, Final>,
        public y_mixin<T, nRows, nColumns, Final>,
        public z_mixin<T, nRows, nColumns, Final>,
        // I J K
        public i_mixin<T, nRows, nColumns, Final>,
        public j_mixin<T, nRows, nColumns, Final>,
        public k_mixin<T, nRows, nColumns, Final>,
        // R G B
        public r_mixin<T, nRows, nColumns, Final>,
        public g_mixin<T, nRows, nColumns, Final>,
        public b_mixin<T, nRows, nColumns, Final>
    {
    };

    // Acessors for four dimensional vectors
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final
    >
    class EDT_EMPTY_BASES Mixin<T, nRows, nColumns, Final,
        std::enable_if_t<is_nd_vector<nRows, nColumns, 4>>> :
        // X Y Z W
        public x_mixin<T, nRows, nColumns, Final>,
        public y_mixin<T, nRows, nColumns, Final>,
        public z_mixin<T, nRows, nColumns, Final>,
        public w_mixin<T, nRows, nColumns, Final>,
        // R G B A
        public r_mixin<T, nRows, nColumns, Final>,
        public g_mixin<T, nRows, nColumns, Final>,
        public b_mixin<T, nRows, nColumns, Final>,
        public a_mixin<T, nRows, nColumns, Final>
    {
    };

#undef EDT_IMPLEMENT_FIELD_MIXIN
}