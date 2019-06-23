#pragma once

#include "Details/Mixins/DataView.h"
#include "Details/Mixins/MatrixMultiplication.h"
#include "Details/Mixins/VectorDataAccess.h"
#include "Details/Mixins/VectorRotation.h"
#include "Details/Mixins/VectorMethods.h"
#include "Details/Mixins/Vector3.h"
#include "Details/Mixins/Cast.h"
#include "Details/Mixins/Common.h"

#include "Matrix.h"
#include "LineMatrixView.h"

namespace edt::geom
{
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns
    >
    class EDT_EMPTY_BASES MatrixView:
        public details::data_view::Mixin<T, nRows, nColumns, ::edt::geom::MatrixView, ::edt::geom::Matrix>,
        public details::matrix_multiplication::Mixin<T, nRows, nColumns, ::edt::geom::MatrixView, ::edt::geom::Matrix>,
        public details::vector_3::Mixin<T, nRows, nColumns, ::edt::geom::MatrixView, ::edt::geom::Matrix>,
        public details::vector_data_access::Mixin<T, nRows, nColumns, ::edt::geom::MatrixView, ::edt::geom::Matrix>,
        public details::vector_rotation::Mixin<T, nRows, nColumns, ::edt::geom::MatrixView, ::edt::geom::Matrix>,
        public details::vector_methods::Mixin<T, nRows, nColumns, ::edt::geom::MatrixView, ::edt::geom::Matrix>,
        public details::cast::Mixin<T, nRows, nColumns, ::edt::geom::MatrixView, ::edt::geom::Matrix>,
        public details::common::Mixin<T, nRows, nColumns, ::edt::geom::MatrixView, ::edt::geom::Matrix>
    {
    };
}
