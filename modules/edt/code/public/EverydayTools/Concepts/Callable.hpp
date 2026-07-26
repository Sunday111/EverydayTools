#pragma once

#include <concepts>

namespace edt
{
template <typename F, typename Ret, typename... Args>
concept Callable = std::invocable<F, Args...> && std::same_as<std::invoke_result_t<F, Args...>, Ret>;
}
