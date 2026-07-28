#pragma once

#include <format>
#include <stdexcept>
#include <utility>

namespace edt
{
template <typename Exception = std::runtime_error, typename... Args>
void ThrowIfFailed(bool condition, std::format_string<Args...> fmt, Args&&... args)
{
    if (!condition)
    {
        throw Exception(std::format(fmt, std::forward<Args>(args)...));
    }
}

// Preferred over the format overload for a literal message: braces stay literal.
template <typename Exception = std::runtime_error>
constexpr void ThrowIfFailed(bool condition, const char* message)
{
    if (!condition)
    {
        throw Exception(message);
    }
}
}  // namespace edt
