#include "edt/template/pure_same_types.hpp"

static_assert(edt::pureSame<int, const int, volatile int, int&, const int&, const volatile int&>);
static_assert(!edt::pureSame<float, int>);
