#pragma once

#include <type_traits>

namespace edt
{
    template<typename T>
    struct enable_enum_flags
    {
        static constexpr bool value = false;
    };

    template<typename T>
    inline constexpr bool enable_enum_flags_v = enable_enum_flags<std::decay_t<T>>::value;
}

template<typename E, typename Enable = std::enable_if_t<edt::enable_enum_flags_v<E>>>
constexpr inline E operator|(const E lhs, const E rhs) {
    using underlying = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template<typename E, typename Enable = std::enable_if_t<edt::enable_enum_flags_v<E>>>
inline E& operator|=(E& lhs, const E rhs) {
    lhs = lhs | rhs;
    return lhs;
}

template<typename E, typename Enable = std::enable_if_t<edt::enable_enum_flags_v<E>>>
constexpr inline E operator&(const E lhs, const E rhs) {
    using underlying = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template<typename E, typename Enable = std::enable_if_t<edt::enable_enum_flags_v<E>>>
inline E& operator&=(E& lhs, const E rhs) {
    lhs = lhs & rhs;
    return lhs;
}

template<typename E, typename Enable = std::enable_if_t<edt::enable_enum_flags_v<E>>>
constexpr inline E operator^(const E lhs, const E rhs) {
    using underlying = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
}

template<typename E, typename Enable = std::enable_if_t<edt::enable_enum_flags_v<E>>>
inline E& operator^=(E& lhs, const E rhs) {
    lhs = lhs ^ rhs;
    return lhs;
}

template<typename E, typename Enable = std::enable_if_t<edt::enable_enum_flags_v<E>>>
constexpr inline E operator~(const E value) {
    using underlying = std::underlying_type_t<E>;
    return static_cast<E>(~static_cast<underlying>(value));
}

template<typename E, typename Enable = std::enable_if_t<edt::enable_enum_flags_v<E>>>
constexpr inline E operator<<(const E value, std::size_t i) {
    using underlying = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<underlying>(value) << i);
}

template<typename E, typename Enable = std::enable_if_t<edt::enable_enum_flags_v<E>>>
inline E& operator<<=(E& value, std::size_t i) {
    value = value << i;
    return value;
}

template<typename E, typename Enable = std::enable_if_t<edt::enable_enum_flags_v<E>>>
constexpr inline E operator>>(const E value, std::size_t i) {
    using underlying = std::underlying_type_t<E>;
    return static_cast<E>(static_cast<underlying>(value) >> i);
}

template<typename E, typename Enable = std::enable_if_t<edt::enable_enum_flags_v<E>>>
inline E& operator>>=(E& value, std::size_t i) {
    value = value >> i;
    return value;
}
