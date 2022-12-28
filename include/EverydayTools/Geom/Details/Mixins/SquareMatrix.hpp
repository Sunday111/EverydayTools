#pragma once

#include "EverydayTools/Geom/Details/ImplementCastThis.hpp"
#include "EverydayTools/Geom/Traits.hpp"

namespace edt::geom::details::square_matrix {
template <typename T, size_t nRows, size_t nColumns,
          template <typename, size_t, size_t> typename Final,
          template <typename, size_t, size_t> typename ReturnValue,
          typename Enable = void>
class Mixin {};

template <typename T, size_t nRows, size_t nColumns,
          template <typename, size_t, size_t> typename Final,
          template <typename, size_t, size_t> typename ReturnValue>
class Mixin<T, nRows, nColumns, Final, ReturnValue,
            std::enable_if_t<is_square<nRows, nColumns>>> {
  EDT_MATRIX_IMPLEMENT_CAST_THIS
 public:
  static decltype(auto) Identity() {
    ReturnValue<T, nRows, nColumns> r;
    for (size_t i = 0; i < nRows; ++i) {
      for (size_t j = 0; j < nColumns; ++j) {
        r.At(i, j) = i == j ? T(1) : T(0);
      }
    }
    return r;
  }

  constexpr bool IsIdentity() const {
    auto& this_ = CastThis();
    for (size_t i = 0; i < nRows; ++i) {
      for (size_t j = 0; j < nColumns; ++j) {
        if (this_.At(i, j) != (i == j ? 1 : 0)) {
          return false;
        }
      }
    }

    return true;
  }
};
}  // namespace edt::geom::details::square_matrix
