#pragma once

#include <cstdint>
#include "../Reflection.h"

inline constexpr edt::GUID int8_guid = edt::GUID::Create("C11C9834-27F0-4DE2-9A2F-73AFDD952130");

void ReflectType(edt::reflection::Reflector<std::int8_t> reflector) {
    reflector.SetGUID(int8_guid);
}

inline constexpr edt::GUID int16_guid = edt::GUID::Create("010AFE8A-43AB-4962-8D53-B9BC934ED9E1");

void ReflectType(edt::reflection::Reflector<std::int16_t> reflector) {
    reflector.SetGUID(int16_guid);
}

inline constexpr edt::GUID int32_guid = edt::GUID::Create("010095BF-2D98-4F64-84F7-64D8096C35B7");

void ReflectType(edt::reflection::Reflector<std::int32_t> reflector) {
    reflector.SetGUID(int32_guid);
}

inline constexpr edt::GUID int64_guid = edt::GUID::Create("80D74E3F-CE87-4D9E-849F-EC47319F513D");

void ReflectType(edt::reflection::Reflector<std::int64_t> reflector) {
    reflector.SetGUID(int64_guid);
}

inline constexpr edt::GUID uint8_guid = edt::GUID::Create("25B87322-55DB-4139-B635-7AE2216C67F0");

void ReflectType(edt::reflection::Reflector<std::uint8_t> reflector) {
    reflector.SetGUID(uint8_guid);
}

inline constexpr edt::GUID uint16_guid = edt::GUID::Create("4A12C3AD-256F-4404-8ECC-C8270C781BD3");

void ReflectType(edt::reflection::Reflector<std::uint16_t> reflector) {
    reflector.SetGUID(uint16_guid);
}

inline constexpr edt::GUID uint32_guid = edt::GUID::Create("5284221F-34C9-4C3B-9D35-6B795E92E57D");

void ReflectType(edt::reflection::Reflector<std::uint32_t> reflector) {
    reflector.SetGUID(uint32_guid);
}

inline constexpr edt::GUID uint64_guid = edt::GUID::Create("828AD754-DC22-425E-BAB3-CF939535D7EE");

void ReflectType(edt::reflection::Reflector<std::uint64_t> reflector) {
    reflector.SetGUID(uint64_guid);
}