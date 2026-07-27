#pragma once

#include <type_traits>
#include <vector>

#include "../template/is_specialization.hpp"
#include "array_view.hpp"

namespace edt
{
template <
    typename VectorSpecialization,
    typename Enable = std::enable_if_t<isSpecialization<std::decay_t<VectorSpecialization>, std::vector>>>
decltype(auto) MakeArrayView(VectorSpecialization& vector) noexcept
{
    using VectorValue = typename VectorSpecialization::value_type;
    using ViewValue =
        std::conditional_t<std::is_const_v<VectorSpecialization>, std::add_const_t<const VectorValue>, VectorValue>;
    using Result = DenseArrayView<ViewValue>;
    if (vector.empty())
    {
        return Result();
    }
    return Result(&vector[0], vector.size());
}
}  // namespace edt
