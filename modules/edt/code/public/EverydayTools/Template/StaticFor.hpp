#pragma once

#include <cstddef>
#include <utility>

namespace edt
{

namespace detail
{
template <std::size_t begin, typename Function, std::size_t... indices>
constexpr void StaticForImpl(Function&& function, std::index_sequence<indices...>)
{
    (static_cast<void>(function.template operator()<begin + indices>()), ...);
}
}  // namespace detail

template <std::size_t begin, std::size_t end, typename Function>
constexpr void StaticFor(Function&& function)
{
    static_assert(begin <= end, "StaticFor begin must not exceed end");
    if constexpr (begin <= end)
    {
        detail::StaticForImpl<begin>(std::forward<Function>(function), std::make_index_sequence<end - begin>{});
    }
}

template <std::size_t count, typename Function>
constexpr void StaticFor(Function&& function)
{
    StaticFor<0, count>(std::forward<Function>(function));
}

}  // namespace edt
