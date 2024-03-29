#pragma once

#include "EverydayTools/Geom/Details/ImplementCastThis.hpp"

namespace edt::geom::details::vector_rotation {
template <typename T, size_t nRows, size_t nColumns,
          template <typename, size_t, size_t> typename Final,
          template <typename, size_t, size_t> typename ReturnValue,
          typename Enable = void>
class Mixin {};

template <typename T, size_t nRows, size_t nColumns,
          template <typename, size_t, size_t> typename Final,
          template <typename, size_t, size_t> typename ReturnValue>
class Mixin<T, nRows, nColumns, Final, ReturnValue,
            std::enable_if_t<is_nd_vector<nRows, nColumns, 2>>> {
  EDT_MATRIX_IMPLEMENT_CAST_THIS
 public:
  TFinal Rotated(T a) const noexcept {
    auto& this_ = CastThis();
    auto cosa = std::cos(a);
    auto sina = std::sin(a);
    TFinal res;
    res.rx() = this_.x() * cosa - this_.y() * sina;
    res.ry() = this_.x() * sina + this_.y() * cosa;
    return res;
  }

  void Rotate(T a) noexcept {
    auto& this_ = CastThis();
    this_ = this_.Rotated(a);
  }
};
}  // namespace edt::geom::details::vector_rotation