#include <EverydayTools/Template/AllTypesMatchCondition.h>

using namespace edt;

static_assert(allTypesMatchCondition<std::is_integral, int, unsigned>);
static_assert(!allTypesMatchCondition<std::is_integral, float, unsigned>);
