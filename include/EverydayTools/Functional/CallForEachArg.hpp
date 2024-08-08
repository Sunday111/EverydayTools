#pragma once

#include <utility>

namespace edt
{
/// Call specified function object F for each specified argument
template <typename Functor, typename... Args>
void CallForEachArg(Functor&& f, Args&&... args)
{
    (f(std::forward<Args>(args)), ...);
}
}  // namespace edt
