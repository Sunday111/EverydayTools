#pragma once

#include "EverydayTools/Geom/Details/Mixins/Data.h"
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
        public details::data_view::Mixin<T, Rows, Columns, ::edt::geom::Matrix>,
        public details::matrix_multiplication::Mixin<T, Rows, Columns, ::edt::geom::Matrix>,
        public details::vector_data_access::Mixin<T, Rows, Columns, ::edt::geom::Matrix>,
        public details::vector_rotation::Mixin<T, Rows, Columns, ::edt::geom::Matrix>,
        public details::vector_methods::Mixin<T, Rows, Columns, ::edt::geom::Matrix>,
        public details::cast::Mixin<T, Rows, Columns, ::edt::geom::Matrix>,
        public details::common::Mixin<T, Rows, Columns, ::edt::geom::Matrix>
    {
    };
}
