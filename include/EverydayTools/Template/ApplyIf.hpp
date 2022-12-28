#pragma once

#include <type_traits>

namespace edt
{
    template
    <
        bool condition,
        template<typename...> typename Modifier,
        typename T
    >
    using ApplyIf = std::conditional_t<condition, Modifier<T>, T>;
}
