#pragma once

#include <cstddef>
#include <type_traits>

namespace edt
{
template <typename T>
struct enable_enum_bitset
{
    static constexpr bool value = false;
};

template <typename T>
inline constexpr bool enable_enum_bitset_v = enable_enum_bitset<std::decay_t<T>>::value;

template <typename T>
concept EnumBitset = std::is_enum_v<std::decay_t<T>> && enable_enum_bitset_v<T>;
}  // namespace edt

// At global scope: the enum being extended lives in the consumer's namespace, so
// ADL would never reach edt.
template <edt::EnumBitset E>
[[nodiscard]] constexpr E operator|(E lhs, E rhs)
{
    using underlying = std::underlying_type_t<E>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange): a flag set holds combinations
    return static_cast<E>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

template <edt::EnumBitset E>
constexpr E& operator|=(E& lhs, E rhs)
{
    lhs = lhs | rhs;
    return lhs;
}

template <edt::EnumBitset E>
[[nodiscard]] constexpr E operator&(E lhs, E rhs)
{
    using underlying = std::underlying_type_t<E>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange): a flag set holds combinations
    return static_cast<E>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}

template <edt::EnumBitset E>
constexpr E& operator&=(E& lhs, E rhs)
{
    lhs = lhs & rhs;
    return lhs;
}

template <edt::EnumBitset E>
[[nodiscard]] constexpr E operator^(E lhs, E rhs)
{
    using underlying = std::underlying_type_t<E>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange): a flag set holds combinations
    return static_cast<E>(static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
}

template <edt::EnumBitset E>
constexpr E& operator^=(E& lhs, E rhs)
{
    lhs = lhs ^ rhs;
    return lhs;
}

template <edt::EnumBitset E>
[[nodiscard]] constexpr E operator~(E value)
{
    using underlying = std::underlying_type_t<E>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange): a flag set holds combinations
    return static_cast<E>(~static_cast<underlying>(value));
}

template <edt::EnumBitset E>
[[nodiscard]] constexpr E operator<<(E value, std::size_t i)
{
    using underlying = std::underlying_type_t<E>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange): a flag set holds combinations
    return static_cast<E>(static_cast<underlying>(value) << i);
}

template <edt::EnumBitset E>
constexpr E& operator<<=(E& value, std::size_t i)
{
    value = value << i;
    return value;
}

template <edt::EnumBitset E>
[[nodiscard]] constexpr E operator>>(E value, std::size_t i)
{
    using underlying = std::underlying_type_t<E>;
    // NOLINTNEXTLINE(clang-analyzer-optin.core.EnumCastOutOfRange): a flag set holds combinations
    return static_cast<E>(static_cast<underlying>(value) >> i);
}

template <edt::EnumBitset E>
constexpr E& operator>>=(E& value, std::size_t i)
{
    value = value >> i;
    return value;
}
