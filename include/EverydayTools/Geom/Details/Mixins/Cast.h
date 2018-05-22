#pragma once

#include "EverydayTools/Geom/Details/ImplementCastThis.h"

namespace edt::geom::details::cast {
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename, size_t, size_t> typename Final,
        template<typename, size_t, size_t> typename ReturnValue,
        typename Enable = void
    >
    class Mixin
    {
        EDT_MATRIX_IMPLEMENT_CAST_THIS
    public:
        template<typename U,
            typename = std::enable_if_t<std::is_convertible_v<T, U>>>
        Final<U, nRows, nColumns> Cast() const noexcept {
            Final<U, nRows, nColumns> that_;
            auto& this_ = CastThis();
            for (size_t i = 0; i < nRows; ++i) {
                for (size_t j = 0; j < nColumns; ++j) {
                    that_.At(i, j) = static_cast<U>(this_.At(i, j));
                }
            }
            return that_;
        }
    };
}