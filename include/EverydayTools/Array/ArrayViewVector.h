#pragma once

#include "ArrayView.h"
#include "EverydayTools/Template/IsSpecialization.h"
#include <vector>

namespace edt
{
    template
    <
        typename VectorSpecialization,
        typename Enable = std::enable_if_t<isSpecialization<VectorSpecialization, std::vector>>
    >
    decltype(auto) MakeArrayView(VectorSpecialization& vector) noexcept {
        using Value = typename VectorSpecialization::value_type;
        using Result = DenseArrayView<Value>;
        if (vector.empty()) {
            return Result();
        }
        return Result(&vector[0], vector.size());
    }
}
