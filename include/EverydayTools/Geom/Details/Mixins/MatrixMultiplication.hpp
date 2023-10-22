#pragma once

#include <cstddef>
#include <type_traits>

#include "EverydayTools/Geom/Details/ImplementCastThis.hpp"

namespace edt::geom::details::matrix_multiplication
{
template <
    typename T,
    size_t nRows,
    size_t nColumns,
    template <typename, size_t, size_t>
    typename Final,
    template <typename, size_t, size_t>
    typename ReturnValue>
class Mixin
{
    EDT_MATRIX_IMPLEMENT_CAST_THIS
public:
    template <typename U, size_t bColumns, template <typename, size_t, size_t> typename Mtx>
    decltype(auto) Multiply(const Mtx<U, nColumns, bColumns>& that_) const
    {
        // nRows is n
        // nColumns is m
        // bColumns is p

        // using TR = decltype(std::declval(T) * std::declval(U));
        using TR = T;
        ReturnValue<TR, nRows, bColumns> result;

        auto& this_ = CastThis();

        for (size_t i = 0; i < nRows; ++i)
        {
            for (size_t j = 0; j < bColumns; ++j)
            {
                auto this_row = this_.GetRow(i);
                auto that_column = that_.GetColumn(j);
                auto dot_prod = this_row.Dot(that_column);
                result.At(i, j) = dot_prod;
            }
        }

        return result;
    }

    template <typename U, size_t bColumns, template <typename, size_t, size_t> typename Mtx>
    decltype(auto) operator*(const Mtx<U, nColumns, bColumns>& that_) const
    {
        return Multiply(that_);
    }

    template <typename U, typename Enable = std::enable_if_t<(std::is_floating_point_v<U> || std::is_integral_v<U>)>>
    constexpr ReturnValue<T, nRows, nColumns> operator*(U value) const noexcept
    {
        ReturnValue<T, nRows, nColumns> result;
        result.Copy(CastThis());
        // auto result = static_cast<ReturnValue<T, nRows, nColumns>>(CastThis());
        // ReturnValue<T, nRows, nColumns> result(CastThis());
        result *= value;
        return result;
    }
};
}  // namespace edt::geom::details::matrix_multiplication
