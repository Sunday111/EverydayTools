#pragma once

namespace edt
{
template <class... Ts>
struct Overload : Ts...
{
    using Ts::operator()...;
};

}  // namespace edt
