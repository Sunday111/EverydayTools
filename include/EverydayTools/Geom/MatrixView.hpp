#pragma once

#include "Details/Mixins/Cast.hpp"
#include "Details/Mixins/Common.hpp"
#include "Details/Mixins/DataView.hpp"
#include "Details/Mixins/MatrixMultiplication.hpp"
#include "Details/Mixins/Vector3.hpp"
#include "Details/Mixins/VectorDataAccess.hpp"
#include "Details/Mixins/VectorMethods.hpp"
#include "Details/Mixins/VectorRotation.hpp"
#include "LineMatrixView.hpp"
#include "Matrix.hpp"

namespace edt::geom {
template <typename T, size_t nRows, size_t nColumns>
class EDT_EMPTY_BASES MatrixView
    : public details::data_view::Mixin<
          T, nRows, nColumns, ::edt::geom::MatrixView, ::edt::geom::Matrix>,
      public details::matrix_multiplication::Mixin<
          T, nRows, nColumns, ::edt::geom::MatrixView, ::edt::geom::Matrix>,
      public details::vector_3::Mixin<
          T, nRows, nColumns, ::edt::geom::MatrixView, ::edt::geom::Matrix>,
      public details::vector_data_access::Mixin<
          T, nRows, nColumns, ::edt::geom::MatrixView, ::edt::geom::Matrix>,
      public details::vector_rotation::Mixin<
          T, nRows, nColumns, ::edt::geom::MatrixView, ::edt::geom::Matrix>,
      public details::vector_methods::Mixin<
          T, nRows, nColumns, ::edt::geom::MatrixView, ::edt::geom::Matrix>,
      public details::cast::Mixin<T, nRows, nColumns, ::edt::geom::MatrixView,
                                  ::edt::geom::Matrix>,
      public details::common::Mixin<T, nRows, nColumns, ::edt::geom::MatrixView,
                                    ::edt::geom::Matrix> {};
}  // namespace edt::geom
