#pragma once

#include "EverydayTools/Geom/Details/Mixins/DataView.h"
#include "EverydayTools/Geom/Matrix.h"

namespace edt::geom
{
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns
    >
    class MatrixView :
        public details::data_view::Mixin<T, Rows, Columns, ::edt::geom::MatrixView>,
        public details::matrix_multiplication::Mixin<T, Rows, Columns, ::edt::geom::MatrixView>,
        public details::vector_data_access::Mixin<T, Rows, Columns, ::edt::geom::MatrixView>,
        public details::vector_rotation::Mixin<T, Rows, Columns, ::edt::geom::MatrixView>,
        public details::vector_methods::Mixin<T, Rows, Columns, ::edt::geom::MatrixView>,
        public details::cast::Mixin<T, Rows, Columns, ::edt::geom::MatrixView>,
        public details::common::Mixin<T, Rows, Columns, ::edt::geom::MatrixView>
    {
    };
}
