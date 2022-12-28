#include "EverydayTools/Template/WorstReference.hpp"

using namespace edt;
static_assert(std::is_same_v<WorstReference_t<int, int&, int&&>, int>);
static_assert(std::is_same_v<WorstReference_t<int&, int&&>, int&>);
static_assert(std::is_same_v<WorstReference_t<int&&, int&&>, int&&>);
static_assert(
    std::is_same_v<WorstReference_t<const int, int&, int&&>, const int>);
static_assert(std::is_same_v<WorstReference_t<const int, volatile int&, int&&>,
                             const int>);
static_assert(std::is_same_v<WorstReference_t<volatile int, const int&, int&&>,
                             volatile int>);
static_assert(
    std::is_same_v<WorstReference_t<const int&&, volatile int&&>, const int&&>);
