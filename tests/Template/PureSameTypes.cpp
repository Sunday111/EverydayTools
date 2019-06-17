#include "EverydayTools/Template/PureSameTypes.h"

using namespace edt;
static_assert(pureSame<int, const int, volatile int, int&, const int&, const volatile int&>);
static_assert(!pureSame<float, int>);
