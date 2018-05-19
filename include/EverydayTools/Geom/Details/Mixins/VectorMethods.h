#pragma once

#include "EverydayTools/Geom/Details/ImplementCastThis.h"

namespace edt::geom::details::vector_methods {
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final,
        typename Enable = void
    >
    class Mixin
    {

    };
    
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final
    >
    class Mixin<T, nRows, nColumns, Final,
        std::enable_if_t<is_vector<nRows, nColumns>>>
    {
        EDT_MATRIX_IMPLEMENT_CAST_THIS
    public:
        static constexpr size_t Size = nRows > nColumns ? nRows : nColumns;
        static constexpr size_t IsRowVector = nRows < nColumns;

        T& Elem(size_t i) {
            auto& result = const_cast<const Mixin*>(this)->Elem(i);
            return const_cast<T&>(result);
        }

        const T& Elem(size_t i) const {
            return AtImpl<IsRowVector>(i);
        }
        
        template
        <
            typename U,
            size_t argRows,
            size_t argColumns,
            template<typename, size_t, size_t> typename Arg,
            class = std::enable_if_t<is_vector<argRows, argColumns>>
        >
        T Dot(const Arg<U, argRows, argColumns>& arg) const {
            T result{};
            for (size_t i = 0; i < Size; ++i) {
                result += Elem(i) * arg.Elem(i);
            }
            return result;
        }

        T& operator[](size_t i) {
            return Elem(i);
        }

        const T& operator[](size_t i) const {
            return Elem(i);
        }

    private:
        template<bool rowVector>
        const T& AtImpl(size_t i) const;

        template<>
        const T& AtImpl<true>(size_t i) const { return CastThis().At(0, i); }

        template<>
        const T& AtImpl<false>(size_t i) const { return CastThis().At(i, 0); }
    };
}