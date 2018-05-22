#pragma once

#include <type_traits>

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

namespace edt::HasMethodTest
{
    ImplementHasMethod(Run);

    struct TrueTest { void Run(void) {} };
    static_assert(HasRunMethod_v<TrueTest>);

    struct TrueTestStatic { static void Run(void) {} };
    static_assert(HasRunMethod_v<TrueTestStatic>);

    struct FalseTest {};
    static_assert(!HasRunMethod_v<FalseTest>);
}
