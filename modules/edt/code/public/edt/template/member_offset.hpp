#pragma once

#include <cstddef>
#include <cstdint>

#include "signature.hpp"

namespace edt
{

// offsetof takes a member name; this takes a pointer to member, so it can be passed around.
template <auto member>
[[nodiscard]] size_t MemberOffset() noexcept
{
    static_assert(Signature<decltype(member)>::IsVariable(), "member has to be a pointer to a data member");
    using Class = typename Signature<decltype(member)>::Class;
    Class const volatile* base = nullptr;
    const auto* field = &(base->*member);
    return static_cast<size_t>(reinterpret_cast<uintptr_t>(field));  // NOLINT
}

}  // namespace edt
