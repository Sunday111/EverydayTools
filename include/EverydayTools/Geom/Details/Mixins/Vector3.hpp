#pragma once

#include "EverydayTools/Geom/Details/ImplementCastThis.hpp"

namespace edt::geom::details::vector_3 {
template <typename T, size_t nRows, size_t nColumns,
          template <typename, size_t, size_t> typename Final,
          template <typename, size_t, size_t> typename ReturnValue,
          typename Enable = void>
class Mixin {};

template <typename T, size_t nRows, size_t nColumns,
          template <typename, size_t, size_t> typename Final,
          template <typename, size_t, size_t> typename ReturnValue>
class Mixin<T, nRows, nColumns, Final, ReturnValue,
            std::enable_if_t<is_nd_vector<nRows, nColumns, 3>>> {
  EDT_MATRIX_IMPLEMENT_CAST_THIS
 public:
  template <typename U, size_t argRows, size_t argColumns,
            template <typename, size_t, size_t> typename Arg,
            class = std::enable_if_t<is_vector<argRows, argColumns>>>
  decltype(auto) Cross(const Arg<U, argRows, argColumns>& that_) const {
    ReturnValue<T, nRows, nColumns> out;
    auto& this_ = CastThis();
    out.Elem(0) = this_.Elem(1) * that_.Elem(2) - this_.Elem(2) * that_.Elem(1);
    out.Elem(1) = this_.Elem(2) * that_.Elem(0) - this_.Elem(0) * that_.Elem(2);
    out.Elem(2) = this_.Elem(0) * that_.Elem(1) - this_.Elem(1) * that_.Elem(0);
    return out;
  }
};
}  // namespace edt::geom::details::vector_3
