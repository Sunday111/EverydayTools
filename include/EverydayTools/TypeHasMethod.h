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
        static const bool Value = std::is_same<std::true_type, decltype(Test<T>(nullptr))>::value;                  \
    }

namespace edt::HasMethodTest
{
    ImplementHasMethod(Run);

    struct TrueTest { void Run(void) {} };
    static_assert(HasRunMethod<TrueTest>::Value);

    struct TrueTestStatic { static void Run(void) {} };
    static_assert(HasRunMethod<TrueTestStatic>::Value);

    struct FalseTest {};
    static_assert(!HasRunMethod<FalseTest>::Value);
}
