#include "edt/template/worst_qualifiers.hpp"

using namespace edt;
static_assert(std::is_same_v<const volatile int, WorstQualifiers_t<const int, volatile int, int> >);
