#include "EverydayTools/Template/WorstQualifiers.h"

using namespace edt;
static_assert(
    std::is_same_v
    <
        const volatile int,
        WorstQualifiers_t<const int, volatile int, int>
    >);
