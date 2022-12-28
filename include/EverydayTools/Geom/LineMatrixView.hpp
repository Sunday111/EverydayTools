#pragma once

#include "Details/FwdDecl.hpp"
#include "Details/Mixins/Cast.hpp"
#include "Details/Mixins/Common.hpp"
#include "Details/Mixins/LineDataView.hpp"
#include "Details/Mixins/MatrixMultiplication.hpp"
#include "Details/Mixins/Vector3.hpp"
#include "Details/Mixins/VectorDataAccess.hpp"
#include "Details/Mixins/VectorMethods.hpp"
#include "Details/Mixins/VectorRotation.hpp"
#include "Matrix.hpp"
#include "MatrixView.hpp"


namespace edt::geom {
// May represent both rows and columns
template <typename T, size_t Rows, size_t Columns>
class EDT_EMPTY_BASES LineMatrixView
    : public details::line_data_view::Mixin<
          T, Rows, Columns, ::edt::geom::LineMatrixView, ::edt::geom::Matrix>,
      public details::matrix_multiplication::Mixin<
          T, Rows, Columns, ::edt::geom::LineMatrixView, ::edt::geom::Matrix>,
      public details::vector_3::Mixin<
          T, Rows, Columns, ::edt::geom::LineMatrixView, ::edt::geom::Matrix>,
      public details::vector_data_access::Mixin<
          T, Rows, Columns, ::edt::geom::LineMatrixView, ::edt::geom::Matrix>,
      public details::vector_rotation::Mixin<
          T, Rows, Columns, ::edt::geom::LineMatrixView, ::edt::geom::Matrix>,
      public details::vector_methods::Mixin<
          T, Rows, Columns, ::edt::geom::LineMatrixView, ::edt::geom::Matrix>,
      public details::cast::Mixin<T, Rows, Columns, ::edt::geom::LineMatrixView,
                                  ::edt::geom::Matrix>,
      public details::common::Mixin<
          T, Rows, Columns, ::edt::geom::LineMatrixView, ::edt::geom::Matrix> {
  using Data = ::edt::geom::details::line_data_view::Mixin<
      T, Rows, Columns, ::edt::geom::LineMatrixView, ::edt::geom::Matrix>;

 public:
  using Data::Data;

  // constexpr operator Matrix<T, Rows, Columns>() const noexcept {
  //     Matrix<T, Rows, Columns> Result;
  //     Result = *this;
  //     return Result;
  // }
};
}  // namespace edt::geom
