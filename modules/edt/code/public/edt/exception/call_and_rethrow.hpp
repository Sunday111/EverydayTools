#pragma once

#include <concepts>
#include <exception>
#include <source_location>
#include <stdexcept>
#include <utility>

namespace edt
{
template <typename Exception = std::runtime_error, std::invocable F>
decltype(auto) CallAndRethrow(F&& f, std::source_location location = std::source_location::current())
{
    try
    {
        return std::forward<F>(f)();
    }
    catch (...)
    {
        std::throw_with_nested(Exception(location.function_name()));
    }
}

template <typename Exception = std::runtime_error, typename TChar, std::invocable F>
decltype(auto) CallAndRethrow(const TChar* rethrow_message, F&& f)
{
    try
    {
        return std::forward<F>(f)();
    }
    catch (...)
    {
        std::throw_with_nested(Exception(rethrow_message));
    }
}
}  // namespace edt
