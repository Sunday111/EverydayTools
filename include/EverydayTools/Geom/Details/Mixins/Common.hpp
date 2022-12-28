#pragma once

#include <type_traits>

#include "../ImplementCastThis.hpp"


namespace edt::geom::details::common {
template <typename T, size_t nRows, size_t nColumns,
          template <typename, size_t, size_t> typename Final,
          template <typename, size_t, size_t> typename ReturnValue>
class Mixin {
  EDT_MATRIX_IMPLEMENT_CAST_THIS
 public:
  template <typename U, template <typename, size_t, size_t> typename Mtx>
  TFinal& Add(const Mtx<U, nRows, nColumns>& that_) noexcept {
    auto& this_ = CastThis();
    for (size_t i = 0; i < nRows; ++i) {
      for (size_t j = 0; j < nColumns; ++j) {
        this_.At(i, j) += that_.At(i, j);
      }
    }
    return this_;
  }

  TFinal& Add(const T& value) noexcept {
    auto& this_ = CastThis();
    for (size_t i = 0; i < nRows; ++i) {
      for (size_t j = 0; j < nColumns; ++j) {
        this_.At(i, j) += value;
      }
    }
    return this_;
  }

  template <typename U>
  TFinal& Multiply(U value) noexcept {
    auto& this_ = CastThis();
    for (size_t i = 0; i < nRows; ++i) {
      for (size_t j = 0; j < nColumns; ++j) {
        this_.At(i, j) *= value;
      }
    }
    return this_;
  }

  TFinal& Fill(const T& value) noexcept {
    auto& this_ = CastThis();
    for (size_t i = 0; i < nRows; ++i) {
      for (size_t j = 0; j < nColumns; ++j) {
        this_.At(i, j) = value;
      }
    }
    return this_;
  }

  template <typename U>
  TFinal& operator*=(U value) noexcept {
    return Multiply(value);
  }

  template <template <typename, size_t, size_t> typename Mtx>
  void Copy(const Mtx<T, nRows, nColumns>& that_) noexcept {
    auto& this_ = CastThis();
    for (size_t r = 0; r < nRows; ++r) {
      for (size_t c = 0; c < nColumns; ++c) {
        this_.At(r, c) = that_.At(r, c);
      }
    }
  }

  template <typename U>
  constexpr ReturnValue<T, nRows, nColumns> operator*(U value) const noexcept {
    ReturnValue<T, nRows, nColumns> result;
    result.Copy(CastThis());
    // auto result = static_cast<ReturnValue<T, nRows, nColumns>>(CastThis());
    // ReturnValue<T, nRows, nColumns> result(CastThis());
    result *= value;
    return result;
  }

  template <typename U, template <typename, size_t, size_t> typename Mtx>
  TFinal& operator+=(const Mtx<U, nRows, nColumns>& that_) noexcept {
    return Add(that_);
  }

  template <typename U, template <typename, size_t, size_t> typename Mtx>
  ReturnValue<T, nRows, nColumns> operator+(
      const Mtx<U, nRows, nColumns>& that_) const noexcept {
    ReturnValue<T, nRows, nColumns> result(CastThis());
    result += that_;
    return result;
  }

  [[nodiscard]] constexpr static size_t RowsCount() noexcept { return nRows; }

  [[nodiscard]] constexpr static size_t ColumnsCount() noexcept {
    return nColumns;
  }
};
}  // namespace edt::geom::details::common
