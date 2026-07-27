#include "edt/template/any_type_match_condition.hpp"

using namespace edt;

static_assert(anyTypeMatchCondition<std::is_integral, int, float>);
static_assert(!anyTypeMatchCondition<std::is_integral, float, double>);
