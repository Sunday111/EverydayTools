#pragma once

namespace edt::reflection
{
    class ArrayOperators
    {
    public:
        using GetSizeOperator = size_t (*)(const void* pObject);
        using ElementAtOperator = void* (*) (void* pObject, size_t index);
        using ElementAtConstOperator = const void* (*) (const void* pObject, size_t index);

        GetSizeOperator getSizeOperator = nullptr;
        ElementAtOperator getElementAtOperator = nullptr;
        ElementAtConstOperator getElementAtConstOperator = nullptr;
    };
}
