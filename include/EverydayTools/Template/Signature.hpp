#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace edt
{

template <typename T>
struct Signature;

// function
template <typename Ret_, typename... Args_>
struct Signature<Ret_ (*)(Args_...)>
{
    using Ret = Ret_;
    using Class = void;
    using Args = std::tuple<Args_...>;
    static constexpr size_t GetArgsCount() { return std::tuple_size_v<Args>; }
    static constexpr bool IsMethod() { return !Pure; }
    static constexpr bool Const = false;
    static constexpr bool Noexcept = false;
    static constexpr bool Volatile = false;
    static constexpr bool Pure = true;
    static constexpr bool LRef = false;
    static constexpr bool RRef = false;
};

// noexcept function
template <typename Ret_, typename... Args_>
struct Signature<Ret_ (*)(Args_...) noexcept> : Signature<Ret_ (*)(Args_...)>
{
    static constexpr bool Noexcept = true;
};

// method
template <typename Ret_, typename Class_, typename... Args_>
struct Signature<Ret_ (Class_::*)(Args_...)> : Signature<Ret_ (*)(Args_...)>
{
    using Class = Class_;
    static constexpr bool Pure = false;
};

#ifndef EDT_SIGNATURE_METHOD_SPECIALIZATION
#define EDT_SIGNATURE_METHOD_SPECIALIZATION(qualifiers)          \
    template <typename Ret_, typename Class_, typename... Args_> \
    struct Signature<Ret_ (Class_::*)(Args_...) qualifiers> : Signature<Ret_ (Class_::*)(Args_...)>
#else
static_assert(false, "Name collision with test macro");
#endif

EDT_SIGNATURE_METHOD_SPECIALIZATION(&)
{
    static constexpr bool LRef = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(&&)
{
    static constexpr bool RRef = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(const)
{
    static constexpr bool Const = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(const&)
{
    static constexpr bool Const = true;
    static constexpr bool LRef = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(const&&)
{
    static constexpr bool Const = true;
    static constexpr bool RRef = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(noexcept)
{
    static constexpr bool Noexcept = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(& noexcept)
{
    static constexpr bool Noexcept = true;
    static constexpr bool LRef = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(&& noexcept)
{
    static constexpr bool Noexcept = true;
    static constexpr bool RRef = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(volatile)
{
    static constexpr bool Volatile = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(volatile&)
{
    static constexpr bool Volatile = true;
    static constexpr bool LRef = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(volatile&&)
{
    static constexpr bool Volatile = true;
    static constexpr bool RRef = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(const noexcept)
{
    static constexpr bool Const = true;
    static constexpr bool Noexcept = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(const& noexcept)
{
    static constexpr bool Const = true;
    static constexpr bool Noexcept = true;
    static constexpr bool LRef = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(const&& noexcept)
{
    static constexpr bool Const = true;
    static constexpr bool Noexcept = true;
    static constexpr bool RRef = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(const volatile)
{
    static constexpr bool Const = true;
    static constexpr bool Volatile = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(const volatile&)
{
    static constexpr bool Const = true;
    static constexpr bool Volatile = true;
    static constexpr bool LRef = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(const volatile&&)
{
    static constexpr bool Const = true;
    static constexpr bool Volatile = true;
    static constexpr bool RRef = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(volatile noexcept)
{
    static constexpr bool Volatile = true;
    static constexpr bool Noexcept = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(volatile& noexcept)
{
    static constexpr bool Volatile = true;
    static constexpr bool Noexcept = true;
    static constexpr bool LRef = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(volatile&& noexcept)
{
    static constexpr bool Volatile = true;
    static constexpr bool Noexcept = true;
    static constexpr bool RRef = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(const volatile noexcept)
{
    static constexpr bool Const = true;
    static constexpr bool Volatile = true;
    static constexpr bool Noexcept = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(const volatile& noexcept)
{
    static constexpr bool Const = true;
    static constexpr bool Volatile = true;
    static constexpr bool Noexcept = true;
    static constexpr bool LRef = true;
};
EDT_SIGNATURE_METHOD_SPECIALIZATION(const volatile&& noexcept)
{
    static constexpr bool Const = true;
    static constexpr bool Volatile = true;
    static constexpr bool Noexcept = true;
    static constexpr bool RRef = true;
};

template <auto ptr>
struct SignatureFromPtr : Signature<decltype(ptr)>
{
};

#undef EDT_SIGNATURE_METHOD_SPECIALIZATION

}  // namespace edt