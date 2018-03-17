#pragma once

#include "ArrayView.h"
#include "EverydayTools/Template/is_specialization.h"
#include <vector>

namespace edt
{
    template<typename VectorSpecialization,
        typename Enable = std::enable_if_t<is_specialization_v<VectorSpecialization, std::vector>>>
        decltype(auto) MakeArrayView(VectorSpecialization& vector) noexcept {
        using Value = typename VectorSpecialization::value_type;
        using Result = DenseArrayView<Value>;
        if (vector.empty()) {
            return Result();
        }
        return Result(&vector[0], vector.size());
    }
}
