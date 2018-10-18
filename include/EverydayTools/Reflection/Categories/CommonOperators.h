#pragma once

namespace edt::reflection
{
    class CommonOperators
    {
    public:
        using DefaultConstructor = void(*)(void* object);
        using CopyConstructor = void(*)(void* objectToConstruct, const void* referenceObject);
        using MoveConstructor = void(*)(void* destinationObject, void* sourceObject);
        using CopyAssign = void(*)(void* destination, const void* source);
        using MoveAssign = void(*)(void* destination, void* source);
        using Equals = bool(*)(const void* lhs, const void* rhs);
        using Destructor = void(*)(void* object);

        DefaultConstructor defaultConstructor = nullptr;
        CopyConstructor copyConstructor = nullptr;
        MoveConstructor moveConstructor = nullptr;
        CopyAssign copyAssign = nullptr;
        MoveAssign moveAssign = nullptr;
        Equals equals = nullptr;
        Destructor destructor = nullptr;
    };
}
