#include "EverydayTools/Template/ApplyIf.hpp"

static_assert(std::is_same_v<edt::ApplyIf<true, std::add_const_t, int>, const int>);
