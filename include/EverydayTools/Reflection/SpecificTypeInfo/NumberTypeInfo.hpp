#pragma once

namespace edt::reflection
{
    enum class NumberType
    {
        SignedInteger,
        UnsignedInteger,
        FloatingPoint
    };

    class NumberTypeInfo
    {
    public:
        NumberType numberType;
    };
}
