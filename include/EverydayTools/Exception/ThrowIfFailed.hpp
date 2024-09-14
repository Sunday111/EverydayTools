#pragma once

#include <sstream>
#include <string>

namespace edt
{
template <typename Exception = std::runtime_error, typename TChar = char, typename... Args>
void ThrowIfFailed(bool condition, Args&&... args)
{
    if (!condition)
    {
        std::basic_stringstream<TChar> stream;
        std::initializer_list<int>{(stream << std::forward<Args>(args), 0)...};
        throw Exception(stream.str());
    }
}
}  // namespace edt
