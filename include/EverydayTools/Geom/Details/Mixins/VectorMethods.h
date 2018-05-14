#pragma once

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
            return AtImpl<IsRowVector>(i);
        }

        T Dot(const TFinal& final) const {
            T result{};
            for (size_t i = 0; i < Size; ++i) {
                result += Elem(i) * final.Elem(i);
            }
        }

    private:
        template<bool rowVector>
        T& AtImpl(size_t i);

        template<>
        T& AtImpl<true>(size_t i) { return CastThis().At(0, i); }

        template<>
        T& AtImpl<false>(size_t i) { return CastThis().At(i, 0); }
    };
}