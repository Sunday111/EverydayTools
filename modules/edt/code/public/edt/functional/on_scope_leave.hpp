#pragma once

#include <utility>

namespace edt
{

template <typename Fn>
auto OnScopeLeave(Fn&& fn)
{
    struct Guard
    {
        // Fn may itself be an lvalue reference, so forwarding rather than unconditionally moving is required.
        explicit Guard(Fn&& on_leave)  // NOLINT(cppcoreguidelines-rvalue-reference-param-not-moved)
            : on_leave_(std::forward<Fn>(on_leave))
        {
        }
        Guard(const Guard&) = delete;
        Guard(Guard&& another) noexcept : on_leave_(std::forward<Fn>(another.on_leave_)) { another.empty_ = true; }
        ~Guard()
        {
            if (!empty_)
            {
                on_leave_();
            }
        }

        Fn on_leave_;
        bool empty_ = false;
    } guard(std::forward<Fn>(fn));

    return guard;
}

}  // namespace edt
