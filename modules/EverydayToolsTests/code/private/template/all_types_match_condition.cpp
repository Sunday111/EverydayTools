#include "edt/template/all_types_match_condition.hpp"

using namespace edt;

static_assert(allTypesMatchCondition<std::is_integral, int, unsigned>);
static_assert(!allTypesMatchCondition<std::is_integral, float, unsigned>);
