#pragma once

#include <type_traits>

/// Creates Has##$name##Method type trait
/* Value will be true for any method signature or return value.
    Target method can also be static.
 */
#define ImplementHasMethod($name)                                                                                   \
    template <typename T>                                                                                           \
    struct Has##$name##Method                                                                                       \
    {                                                                                                               \
        template <class, class> class checker;                                                                      \
                                                                                                                    \
        template <typename C>                                                                                       \
        static std::true_type Test(checker<C, decltype(&C::$name)> *);                                              \
                                                                                                                    \
        template <typename C>                                                                                       \
        static std::false_type Test(...);                                                                           \
                                                                                                                    \
        typedef decltype(Test<T>(nullptr)) type;                                                                    \
        static const bool Value = std::is_same_v<std::true_type, decltype(Test<T>(nullptr))>;                       \
    };                                                                                                              \
    template<typename T>                                                                                            \
    constexpr bool Has##$name##Method_v = Has##$name##Method<T>::Value
