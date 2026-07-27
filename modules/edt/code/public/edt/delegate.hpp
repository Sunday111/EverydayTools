#pragma once

namespace edt
{
template <typename T>
class Delegate
{
};

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
        m_instance = nullptr;
        m_function = [](void*, Args... args)
        {
            return static_cast<R>(fn(std::forward<Args>(args)...));
        };
    }

    template <typename T, MethodType<T> method>
    void Bind(T* instance)
    {
        m_instance = instance;
        m_function = [](void* object, Args... args)
        {
            return static_cast<R>((static_cast<T*>(object)->*method)(std::forward<Args>(args)...));
        };
    }

    template <typename T>
    void Bind(T& functor)
    {
        m_instance = &functor;
        m_function = [](void* object, Args... args)
        {
            return static_cast<R>((*static_cast<T*>(object))(std::forward<Args>(args)...));
        };
    }

    R Invoke(Args... args) const { return static_cast<R>(m_function(m_instance, std::forward<Args>(args)...)); }

private:
    void* m_instance = nullptr;
    InternalFunctionType m_function = nullptr;
};
}  // namespace edt