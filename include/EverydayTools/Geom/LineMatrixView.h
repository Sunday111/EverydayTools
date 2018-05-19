#pragma once

#include "Details/Mixins/LineDataView.h"
#include "Details/Mixins/MatrixMultiplication.h"
#include "Details/Mixins/VectorDataAccess.h"
#include "Details/Mixins/VectorRotation.h"
#include "Details/Mixins/VectorMethods.h"
#include "Details/Mixins/Cast.h"
#include "Details/Mixins/Common.h"

namespace edt::geom
{
    // May represent both rows and columns
    template<typename T, size_t Rows, size_t Columns>
    class LineMatrixView :
        public details::line_data_view::Mixin<T, Rows, Columns, ::edt::geom::LineMatrixView>,
        public details::matrix_multiplication::Mixin<T, Rows, Columns, ::edt::geom::LineMatrixView>,
        public details::vector_data_access::Mixin<T, Rows, Columns, ::edt::geom::LineMatrixView>,
        public details::vector_rotation::Mixin<T, Rows, Columns, ::edt::geom::LineMatrixView>,
        public details::vector_methods::Mixin<T, Rows, Columns, ::edt::geom::LineMatrixView>,
        public details::cast::Mixin<T, Rows, Columns, ::edt::geom::LineMatrixView>,
        public details::common::Mixin<T, Rows, Columns, ::edt::geom::LineMatrixView>
    {
        using Data = ::edt::geom::details::line_data_view::Mixin<T, Rows, Columns, ::edt::geom::LineMatrixView>;
    public:
        using Data::Data;
    };
}
