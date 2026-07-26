#include "EverydayTools/Template/AnyTypeMatchCondition.hpp"

using namespace edt;

static_assert(anyTypeMatchCondition<std::is_integral, int, float>);
static_assert(!anyTypeMatchCondition<std::is_integral, float, double>);
