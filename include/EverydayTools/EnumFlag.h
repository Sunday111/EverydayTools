#pragma once

#include <type_traits>

#define _EDT_ENUM_FLAG_OPERATORS(PREFIX, ET_) \
    PREFIX ET_ operator|(ET_ a, ET_ b) { \
        return static_cast<ET_>(static_cast<std::underlying_type<ET_>::type>(a) | \
                                static_cast<std::underlying_type<ET_>::type>(b)); } \
    PREFIX ET_ operator&(ET_ a, ET_ b) { \
        return static_cast<ET_>(static_cast<std::underlying_type<ET_>::type>(a) & \
                                static_cast<std::underlying_type<ET_>::type>(b)); } \
    PREFIX ET_ operator^(ET_ a, ET_ b) { \
        return static_cast<ET_>(static_cast<std::underlying_type<ET_>::type>(a) ^ \
                                static_cast<std::underlying_type<ET_>::type>(b)); } \
    PREFIX ET_ operator~(ET_ a) { \
        return static_cast<ET_>(~static_cast<std::underlying_type<ET_>::type>(a)); } \
    PREFIX ET_ operator<<(ET_ a, int i) { \
        return static_cast<ET_>(static_cast<std::underlying_type<ET_>::type>(a) << i); } \
    PREFIX ET_ operator>>(ET_ a, int i) { \
        return static_cast<ET_>(static_cast<std::underlying_type<ET_>::type>(a) >> i); } \
    PREFIX ET_ operator|=(ET_& a, ET_ b) { \
        return static_cast<ET_>(reinterpret_cast<std::underlying_type<ET_>::type&>(a) |= \
                                static_cast<std::underlying_type<ET_>::type>(b)); } \
    PREFIX ET_ operator&=(ET_& a, ET_ b) { \
        return static_cast<ET_>(reinterpret_cast<std::underlying_type<ET_>::type&>(a) &= \
                                static_cast<std::underlying_type<ET_>::type>(b)); } \
    PREFIX ET_ operator^=(ET_& a, ET_ b) { \
        return static_cast<ET_>(reinterpret_cast<std::underlying_type<ET_>::type&>(a) ^= \
                                static_cast<std::underlying_type<ET_>::type>(b)); } \
    PREFIX ET_ operator<<=(ET_& a, int i) { \
        return static_cast<ET_>(reinterpret_cast<std::underlying_type<ET_>::type&>(a) <<= i); } \
    PREFIX ET_ operator>>=(ET_& a, int i) { \
        return static_cast<ET_>(reinterpret_cast<std::underlying_type<ET_>::type&>(a) >>= i); }

#define EDT_ENUM_FLAG_OPERATORS(ET_) \
    _EDT_ENUM_FLAG_OPERATORS(inline, ET_)

#define EDT_ENUM_FLAG_OPERATORS_CLASS(ET_) \
    _EDT_ENUM_FLAG_OPERATORS(friend inline, ET_)
