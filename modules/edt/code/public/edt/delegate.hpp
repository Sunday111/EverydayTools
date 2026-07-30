#pragma once

#include <cassert>
#include <utility>

namespace edt
{
template <typename T>
class Delegate;

template <typename R, typename... Args>
class Delegate<R(Args...)>
{
public:
    using FunctionType = R (*)(Args...);

    using InternalFunctionType = R (*)(void*, Args...);

    template <typename Class>
    using MethodType = R (Class::*)(Args...);

    template <FunctionType fn>
    void Bind()
    {
        instance_ = nullptr;
        function_ = [](void*, Args... args)
        {
            return static_cast<R>(fn(std::forward<Args>(args)...));
        };
    }

    template <typename T, MethodType<T> method>
    void Bind(T* instance)
    {
        instance_ = instance;
        function_ = [](void* object, Args... args)
        {
            return static_cast<R>((static_cast<T*>(object)->*method)(std::forward<Args>(args)...));
        };
    }

    template <typename T>
    void Bind(T& functor)
    {
        instance_ = &functor;
        function_ = [](void* object, Args... args)
        {
            return static_cast<R>((*static_cast<T*>(object))(std::forward<Args>(args)...));
        };
    }

    void Reset() noexcept
    {
        instance_ = nullptr;
        function_ = nullptr;
    }

    [[nodiscard]] bool IsBound() const noexcept { return function_ != nullptr; }

    [[nodiscard]] explicit operator bool() const noexcept { return IsBound(); }

    R Invoke(Args... args) const
    {
        assert(function_ != nullptr);  // NOLINT(cert-dcl03-c, misc-static-assert): runtime guard
        return static_cast<R>(function_(instance_, std::forward<Args>(args)...));
    }

    R operator()(Args... args) const { return Invoke(std::forward<Args>(args)...); }

private:
    void* instance_ = nullptr;
    InternalFunctionType function_ = nullptr;
};
}  // namespace edt
