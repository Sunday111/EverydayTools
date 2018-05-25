#pragma once

#include <cmath>
#include <type_traits>

#include "EverydayTools/Preprocessor/EmptyBases.h"

#include "Details/Mixins/Cast.h"
#include "Details/Mixins/Common.h"
#include "Details/Mixins/Data.h"
#include "Details/Mixins/MatrixMultiplication.h"
#include "Details/Mixins/NotVectorMethods.h"
#include "Details/Mixins/Vector3.h"
#include "Details/Mixins/VectorDataAccess.h"
#include "Details/Mixins/VectorMethods.h"
#include "Details/Mixins/VectorRotation.h"
#include "Details/Mixins/SquareMatrix.h"

#include "MatrixView.h"
#include "LineMatrixView.h"

namespace edt::geom {
    template<typename T, size_t Rows, size_t Columns>
    class EDT_EMPTY_BASES Matrix :
        public details::data::Mixin<T, Rows, Columns, ::edt::geom::Matrix, ::edt::geom::Matrix>,
        public details::matrix_multiplication::Mixin<T, Rows, Columns, ::edt::geom::Matrix, ::edt::geom::Matrix>,
        public details::not_vector_methods::Mixin<T, Rows, Columns, ::edt::geom::Matrix, ::edt::geom::Matrix>,
        public details::vector_3::Mixin<T, Rows, Columns, ::edt::geom::Matrix, ::edt::geom::Matrix>,
        public details::vector_data_access::Mixin<T, Rows, Columns, ::edt::geom::Matrix, ::edt::geom::Matrix>,
        public details::vector_rotation::Mixin<T, Rows, Columns, ::edt::geom::Matrix, ::edt::geom::Matrix>,
        public details::vector_methods::Mixin<T, Rows, Columns, ::edt::geom::Matrix, ::edt::geom::Matrix>,
        public details::cast::Mixin<T, Rows, Columns, ::edt::geom::Matrix, ::edt::geom::Matrix>,
        public details::common::Mixin<T, Rows, Columns, ::edt::geom::Matrix, ::edt::geom::Matrix>,
        public details::square_matrix::Mixin<T, Rows, Columns, ::edt::geom::Matrix, ::edt::geom::Matrix>
    {
    private:
        void SizeCheck__() {
            static_assert(sizeof(*this) == sizeof(T) * Rows * Columns);
        }
    };
}

#ifdef EDT_MATRIX_IMPLEMENT_CAST_THIS
#undef EDT_MATRIX_IMPLEMENT_CAST_THIS
#endif
