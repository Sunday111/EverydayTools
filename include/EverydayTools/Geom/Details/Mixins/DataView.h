#pragma once

namespace edt::geom::data_view
{
    template
    <
        typename T,
        size_t nRows,
        size_t nColumns,
        template<typename T, size_t, size_t> typename Final
    >
    class Mixin
    {
    public:
        T* basePointer = nullptr;
        size_t columnsStride = 0;
        size_t rowStride = 0;
    };
}