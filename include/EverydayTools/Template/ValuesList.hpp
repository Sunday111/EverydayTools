#pragma once

#include <type_traits>
#include <utility>

#include "../Size.hpp"
#include "FwdDecl.hpp"

namespace edt
{
template <auto... Values>
class ValuesList
{
};

template <auto... A, auto... B>
constexpr auto ValuesList_Concat(ValuesList<A...>, ValuesList<B...>)
{
    return ValuesList<A..., B...>();
}

template <typename A, typename B>
using ValuesList_ConcatT = decltype(ValuesList_Concat(std::declval<A>(), std::declval<B>()));

template <template <auto> typename F, auto... Values>
constexpr auto ValuesList_MapToType(ValuesList<Values...>)
{
    return TypesList<F<Values>...>();
}

template <template <auto> typename F, typename List>
using ValuesList_MapToTypeT = decltype(ValuesList_MapToType<F>(std::declval<List>()));

template <template <auto> typename F, auto... Values>
constexpr auto ValuesList_MapToValue(ValuesList<Values...>)
{
    return ValuesList<F<Values>::Value...>();
}

template <template <auto> typename F, typename List>
using ValuesList_MapToValueT = decltype(ValuesList_MapToValue<F>(std::declval<List>()));

namespace values_list_filter_impl
{
template <template <auto> typename Predicate, auto...>
struct Impl;

template <template <auto> typename Predicate, auto Head, auto... Tail>
struct Impl<Predicate, Head, Tail...>
{
    using FilteredHead = std::conditional_t<Predicate<Head>::Value, ValuesList<Head>, ValuesList<>>;
    using FilteredTail = typename Impl<Predicate, Tail...>::Result;
    using Result = ValuesList_ConcatT<FilteredHead, FilteredTail>;
};

template <template <auto> typename Predicate, auto Head>
struct Impl<Predicate, Head>
{
    using FilteredHead = std::conditional_t<Predicate<Head>::Value, ValuesList<Head>, ValuesList<>>;
    using Result = FilteredHead;
};

template <template <auto> typename Predicate>
struct Impl<Predicate>
{
    using Result = ValuesList<>;
};
}  // namespace values_list_filter_impl

template <template <auto> typename Predicate, auto... Values>
constexpr auto ValuesList_Filter(ValuesList<Values...>)
{
    using namespace values_list_filter_impl;
    using R = typename Impl<Predicate, Values...>::Result;
    return R{};
}

template <template <auto> typename Predicate, typename List>
using ValuesList_FilterT = decltype(ValuesList_Filter<Predicate>(std::declval<List>()));

template <typename T, T... values>
constexpr auto ValuesList_ToIntegerSequence(ValuesList<values...>)
{
    return std::integer_sequence<T, values...>();
}

template <typename T, typename List>
using ValuesList_ToIntegerSequenceT = decltype(ValuesList_ToIntegerSequence<T>(std::declval<List>()));

template <typename T, T... values>
constexpr auto ValuesList_FromIntegerSequence(std::integer_sequence<T, values...>)
{
    return ValuesList<values...>{};
}

template <typename List>
using ValuesList_FromIntegerSequenceT = decltype(ValuesList_FromIntegerSequence(std::declval<List>()));
}  // namespace edt
