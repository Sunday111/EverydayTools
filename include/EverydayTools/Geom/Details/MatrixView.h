#pragma once

#include "EverydayTools/Geom/Details/Mixins/DataView.h"
#include "EverydayTools/Geom/Details/Mixins/MatrixMultiplication.h"
#include "EverydayTools/Geom/Details/Mixins/VectorDataAccess.h"
#include "EverydayTools/Geom/Details/Mixins/VectorRotation.h"
#include "EverydayTools/Geom/Details/Mixins/VectorMethods.h"
#include "EverydayTools/Geom/Details/Mixins/Cast.h"
#include "EverydayTools/Geom/Details/Mixins/Common.h"

namespace edt::geom
{
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns
    >
    class MatrixView :
        public details::data_view::Mixin<T, nRows, nColumns, ::edt::geom::MatrixView>,
        public details::matrix_multiplication::Mixin<T, nRows, nColumns, ::edt::geom::MatrixView>,
        public details::vector_data_access::Mixin<T, nRows, nColumns, ::edt::geom::MatrixView>,
        public details::vector_rotation::Mixin<T, nRows, nColumns, ::edt::geom::MatrixView>,
        public details::vector_methods::Mixin<T, nRows, nColumns, ::edt::geom::MatrixView>,
        public details::cast::Mixin<T, nRows, nColumns, ::edt::geom::MatrixView>,
        public details::common::Mixin<T, nRows, nColumns, ::edt::geom::MatrixView>
    {
    };
}
