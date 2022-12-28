#pragma once

#include "EverydayTools/Geom/Details/ImplementCastThis.hpp"

namespace edt::geom::details::data_view {
template <typename T, size_t nRows, size_t nColumns,
          template <typename, size_t, size_t> typename Final,
          template <typename, size_t, size_t> typename ReturnValue>
class Mixin {
  EDT_MATRIX_IMPLEMENT_CAST_THIS
 public:
  T& At(size_t nRow, size_t nColumn) noexcept {
    return const_cast<T&>(const_cast<const Mixin*>(this)->At(nRow, nColumn));
  }

  const T& At(size_t nRow, size_t nColumn) const noexcept {
    auto rowOffset = (startRow + rowStride * nRow) * parentRowSize;
    auto columnOffset = startColumn + columnStride * nColumn;
    return GetData()[rowOffset + columnOffset];
  }

  T* GetData() { return parentData; }

  const T* GetData() const { return parentData; }

  Final<T, 1, nColumns> GetRow(size_t row) {
    auto result = CastThis();
    result.startRow += row;
    return result;
  }

  Final<T, nRows, 1> GetColumn(size_t column) {
    auto result = CastThis();
    result.startColumn += column;
    return result;
  }

  T* parentData = nullptr;
  size_t startRow = 0;
  size_t startColumn = 0;
  size_t parentRowSize = 0;
  size_t rowStride = 1;
  size_t columnStride = 1;
};
}  // namespace edt::geom::details::data_view