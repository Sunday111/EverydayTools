#pragma once

#include <sstream>
#include <string>

namespace edt
{
    template<typename Exception = std::runtime_error, typename... Args>
    void ThrowIfFailed(bool condition, Args&&... args) {
        if (!condition) {
            std::stringstream stream;
            std::initializer_list<int> {
                (stream << std::forward<Args>(args), 0)...
            };
            throw Exception(stream.str());
        }
    }
}

