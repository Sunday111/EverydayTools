#pragma once

#include <cmath>
#include <type_traits>

#define EDT_MATRIX_IMPLEMENT_CAST_THIS              \
    using TFinal = Final<T, nRows, nColumns>;       \
    TFinal& CastThis() noexcept {                   \
        return static_cast<TFinal&>(*this);         \
    }                                               \
                                                    \
    const TFinal& CastThis() const noexcept {       \
        return static_cast<const TFinal&>(*this);   \
    }

#include "Details/IsNdVector.h"
#include "Details/EmptyBases.h"
#include "Details/Mixins/Data.h"
#include "Details/Mixins/DataAccess.h"
#include "Details/Mixins/VectorRotation.h"
#include "Details/Mixins/VectorMethods.h"
#include "Details/Mixins/Cast.h"
#include "Details/Mixins/Common.h"

namespace edt::geom {
    template<typename T, size_t Rows, size_t Columns>
    class _EMPTY_BASES Matrix :
        public details::data::Mixin<T, Rows, Columns, ::edt::geom::Matrix>,
        public details::vector_data_access::Mixin<T, Rows, Columns, ::edt::geom::Matrix>,
        public details::vector_rotation::Mixin<T, Rows, Columns, ::edt::geom::Matrix>,
        public details::vector_methods::Mixin<T, Rows, Columns, ::edt::geom::Matrix>,
        public details::cast::Mixin<T, Rows, Columns, ::edt::geom::Matrix>,
        public details::common::Mixin<T, Rows, Columns, ::edt::geom::Matrix>
    {
    private:
        void SizeCheck__() {
            static_assert(sizeof(*this) == sizeof(T) * Rows * Columns);
        }
    };
}

#undef EDT_MATRIX_IMPLEMENT_CAST_THIS
