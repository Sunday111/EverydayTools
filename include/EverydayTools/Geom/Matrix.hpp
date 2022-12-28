#pragma once

#include <cmath>
#include <type_traits>

#include "Details/Mixins/Cast.hpp"
#include "Details/Mixins/Common.hpp"
#include "Details/Mixins/Data.hpp"
#include "Details/Mixins/MatrixMultiplication.hpp"
#include "Details/Mixins/NotVectorMethods.hpp"
#include "Details/Mixins/SquareMatrix.hpp"
#include "Details/Mixins/Vector3.hpp"
#include "Details/Mixins/VectorDataAccess.hpp"
#include "Details/Mixins/VectorMethods.hpp"
#include "Details/Mixins/VectorRotation.hpp"
#include "EverydayTools/Preprocessor/EmptyBases.hpp"
#include "LineMatrixView.hpp"
#include "MatrixView.hpp"

namespace edt::geom {
template <typename T, size_t Rows, size_t Columns>
class EDT_EMPTY_BASES Matrix
    : public details::data::Mixin<T, Rows, Columns, ::edt::geom::Matrix,
                                  ::edt::geom::Matrix>,
      public details::matrix_multiplication::Mixin<
          T, Rows, Columns, ::edt::geom::Matrix, ::edt::geom::Matrix>,
      public details::not_vector_methods::Mixin<
          T, Rows, Columns, ::edt::geom::Matrix, ::edt::geom::Matrix>,
      public details::vector_3::Mixin<T, Rows, Columns, ::edt::geom::Matrix,
                                      ::edt::geom::Matrix>,
      public details::vector_data_access::Mixin<
          T, Rows, Columns, ::edt::geom::Matrix, ::edt::geom::Matrix>,
      public details::vector_rotation::Mixin<
          T, Rows, Columns, ::edt::geom::Matrix, ::edt::geom::Matrix>,
      public details::vector_methods::Mixin<
          T, Rows, Columns, ::edt::geom::Matrix, ::edt::geom::Matrix>,
      public details::cast::Mixin<T, Rows, Columns, ::edt::geom::Matrix,
                                  ::edt::geom::Matrix>,
      public details::common::Mixin<T, Rows, Columns, ::edt::geom::Matrix,
                                    ::edt::geom::Matrix>,
      public details::square_matrix::Mixin<
          T, Rows, Columns, ::edt::geom::Matrix, ::edt::geom::Matrix> {
 private:
  void SizeCheck__() {
    static_assert(sizeof(*this) == sizeof(T) * Rows * Columns);
  }
};
}  // namespace edt::geom

#ifdef EDT_MATRIX_IMPLEMENT_CAST_THIS
#undef EDT_MATRIX_IMPLEMENT_CAST_THIS
#endif
