#pragma once

#include <type_traits>

namespace edt
{
template <typename... Types>
constexpr bool template_and()
{
    return (Types::value && ...);
}

template <template <typename...> typename Trait, typename... Args>
struct AllTypesMatchCondition
    : public std::conditional_t<template_and<Trait<Args>...>(), std::true_type, std::false_type>
{
};

template <template <typename...> typename Trait, typename... Args>
inline constexpr bool allTypesMatchCondition = AllTypesMatchCondition<Trait, Args...>::value;
}  // namespace edt
