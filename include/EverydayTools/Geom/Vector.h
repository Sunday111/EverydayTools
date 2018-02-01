#pragma once

#include "Matrix.h"

namespace edt::geom {
    template<typename T, size_t N>
    using Vector = Matrix<T, 1, N>;
}