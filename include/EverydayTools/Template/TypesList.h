#pragma once

#include <tuple>
#include <type_traits>
#include "FwdDecl.h"

namespace edt
{
    template<typename... Types>
    class TypesList
    {
    public:
        using TupleType = std::tuple<Types...>;
    };

    template<typename... A, typename... B>
    constexpr auto TypesList_Concat(TypesList<A...>, TypesList<B...>) {
        return TypesList<A..., B...>();
    }

    template<typename A, typename B>
    using TypesList_ConcatT = decltype(TypesList_Concat(std::declval<A>(), std::declval<B>()));

    template
    <
        template<typename> typename F,
        typename... Types
    >
    constexpr auto TypesList_MapToType(TypesList<Types...>) {
        return TypesList<F<Types>...>();
    }
    
    template
    <
        template<typename> typename F,
        typename List
    >
    using TypesList_MapToTypeT = decltype(TypesList_MapToType<F>(std::declval<List>()));

    template
    <
        template<typename> typename F,
        typename... Types
    >
    constexpr auto TypesList_MapToValue(TypesList<Types...>) {
        return ValuesList<F<Types>::Value...>();
    }
    
    template
    <
        template<typename> typename F,
        typename List
    >
    using TypesList_MapToValueT = decltype(TypesList_MapToValue<F>(std::declval<List>()));

    namespace types_list_impl
    {
        template
        <
            template<typename> typename Predicate,
            typename... A
        >
        struct TypeList_FilterImpl;

        template
        <
            template<typename> typename Predicate,
            typename Head,
            typename... Tail
        >
        struct TypeList_FilterImpl<Predicate, Head, Tail...>
        {
            using FilteredHead = std::conditional_t<Predicate<Head>::Value, TypesList<Head>, TypesList<>>;
            using FilteredTail = typename TypeList_FilterImpl<Predicate, Tail...>::Result;
            using Result = TypesList_ConcatT<FilteredHead, FilteredTail>;
        };

        template
        <
            template<typename> typename Predicate,
            typename Head
        >
        struct TypeList_FilterImpl<Predicate, Head>
        {
            using FilteredHead = std::conditional_t<Predicate<Head>::Value, TypesList<Head>, TypesList<>>;
            using Result = FilteredHead;
        };

        template
        <
            template<typename> typename Predicate
        >
        struct TypeList_FilterImpl<Predicate>
        {
            using Result = TypesList<>;
        };
    }

    template
    <
        template<typename> typename Predicate,
        typename... Types
    >
    constexpr auto TypesList_Filter(TypesList<Types...>) {
        using namespace types_list_impl;
        using R = typename TypeList_FilterImpl<Predicate, Types...>::Result;
        return R{};
    }

    template
    <
        template<typename> typename Predicate,
        typename List
    >
    using TypesList_FilterT = decltype(TypesList_Filter<Predicate>(std::declval<List>()));
}