#pragma once

#include "../../Template/TypeHasMethod.h"

namespace edt::reflection
{
    template<typename T>
    class TypeInfo;
}

namespace edt::reflection::detail
{
    ImplementHasMethod(ReflectType);

    template<typename T>
    inline constexpr bool TypeIsReflectedWithMethod = HasReflectTypeMethod_v<T>;

    template<typename T>
    class ReflectTypeOverloadChecker
    {
        template<typename = decltype(ReflectType(std::declval<TypeInfo<T>&>()))>
        static std::true_type test(int);

        static std::false_type test(...);

    public:
        static constexpr bool Value = decltype(test(0))::value;
    };

    template<typename T>
    inline constexpr bool TypeIsReflectedWithOverload = ReflectTypeOverloadChecker<T>::Value;

    template<typename T>
    void CallReflectType(TypeInfo<T>& typeInfo) {
        constexpr bool overload = TypeIsReflectedWithOverload<T>;
        constexpr bool method = TypeIsReflectedWithMethod<T>;

        static_assert(overload || method, "Type must be reflected");

        if constexpr (overload) {
            ReflectType(typeInfo);
        } else {
            T::ReflectType(typeInfo);
        }
    }
}

namespace edt::reflection
{
    template<typename T>
    inline constexpr bool TypeIsReflected = detail::TypeIsReflectedWithMethod<T> || detail::TypeIsReflectedWithOverload<T>;
}
