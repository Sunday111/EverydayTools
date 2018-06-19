#pragma once

#include <type_traits>
#include "../ImplementCastThis.h"

namespace edt::geom::details::common {
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename, size_t, size_t> typename Final,
        template<typename, size_t, size_t> typename ReturnValue
    >
    class Mixin
    {
        EDT_MATRIX_IMPLEMENT_CAST_THIS
    public:
        template
        <
            typename U,
            template<typename, size_t, size_t> typename Mtx
        >
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
        
        template<typename U>
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

        template<typename U>
        TFinal& operator*=(U value) noexcept {
            return Multiply(value);
        }

        template<typename U>
        ReturnValue<T, nRows, nColumns> operator*(U value) const noexcept {
            ReturnValue<T, nRows, nColumns> result(CastThis());
            result *= value;
            return result;
        }

        template
        <
            typename U,
            template<typename, size_t, size_t> typename Mtx
        >
        TFinal& operator+=(const Mtx<U, nRows, nColumns>& that_) noexcept {
            return Add(that_);
        }
        
        template
        <
            typename U,
            template<typename, size_t, size_t> typename Mtx
        >
        ReturnValue<T, nRows, nColumns> operator+(const Mtx<U, nRows, nColumns>& that_) const noexcept {
            ReturnValue<T, nRows, nColumns> result(CastThis());
            result += that_;
            return result;
        }

        static size_t RowsCount() noexcept { return nRows; }
        static size_t ColumnsCount() noexcept { return nColumns; }
    };
}
