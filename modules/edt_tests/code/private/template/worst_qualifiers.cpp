#include "edt/template/worst_qualifiers.hpp"

static_assert(std::is_same_v<const volatile int, edt::WorstQualifiers_t<const int, volatile int, int> >);
