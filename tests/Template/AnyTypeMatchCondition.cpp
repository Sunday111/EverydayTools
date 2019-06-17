#include <EverydayTools/Template/AnyTypeMatchCondition.h>

using namespace edt;

static_assert(anyTypeMatchCondition<std::is_integral, int, float>);
static_assert(!anyTypeMatchCondition<std::is_integral, float, double>);
