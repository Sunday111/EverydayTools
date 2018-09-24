#pragma once

#include "../Reflection.h"

inline constexpr edt::GUID float_guid = edt::GUID::Create("1989346D-9F3C-4F48-91E9-978B96BE4084");

void ReflectType(edt::reflection::Reflector<float> reflector)
{
    reflector.SetGUID(float_guid);
}

inline constexpr edt::GUID double_guid = edt::GUID::Create("F9C5FC16-B14B-43E3-B545-DCEFA6544C95");

void ReflectType(edt::reflection::Reflector<double> reflector)
{
    reflector.SetGUID(double_guid);
}
