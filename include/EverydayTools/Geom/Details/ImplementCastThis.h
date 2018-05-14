#pragma once

#define EDT_MATRIX_IMPLEMENT_CAST_THIS              \
    using TFinal = Final<T, nRows, nColumns>;       \
    TFinal& CastThis() noexcept {                   \
        return static_cast<TFinal&>(*this);         \
    }                                               \
                                                    \
    const TFinal& CastThis() const noexcept {       \
        return static_cast<const TFinal&>(*this);   \
    }