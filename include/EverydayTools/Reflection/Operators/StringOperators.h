#pragma once

namespace edt::reflection
{
    class StringOperators
    {
    public:
        using GetValueOperator = const char* (*)(const void* pObject);
        using SetValueOperator = void (*)(void* pObject, const char* value);

        GetValueOperator getValueOperator = nullptr;
        SetValueOperator setValueOperator = nullptr;
    };
}