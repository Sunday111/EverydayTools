#pragma once

#include <stdexcept>

template <typename Exception = std::runtime_error, typename TChar, typename F>
decltype(auto) CallAndRethrow(const TChar* rethrowMessage, F&& f)
{
    try
    {
        // Call passed functor and return it's result
        // Need to use this static cast to handle void return value case
        return static_cast<decltype(f.operator()())>(f());
    }
    catch (...)
    {
        // Wrap exception
        std::throw_with_nested(Exception(rethrowMessage));
    }
}

namespace call_and_rethrow_helpers
{
// This class created to be used in 'CallAndRethrowM' macro.
// It has overloaded template operator+ to accept functors.
// Need it to avoid passing whole lambda text as macro argument
// (because it leads to syntax errors sometimes)
class Caller
{
public:
    constexpr Caller(const char* name) : m_name(name) {}
    template <typename Functor>
    inline decltype(auto) operator+(Functor&& functor) const
    {
        return CallAndRethrow(m_name, std::forward<Functor>(functor));
    }

private:
    const char* const m_name = nullptr;
};
}  // namespace call_and_rethrow_helpers

#ifdef _MSC_VER
#define CallAndRethrowM call_and_rethrow_helpers::Caller(__FUNCTION__)
#elif
// Need some warning here?
#define CallAndRethrowM call_and_rethrow_helpers::Caller(__func__)
#endif