#include "edt/template/worst_reference.hpp"

static_assert(std::is_same_v<edt::WorstReference_t<int, int&, int&&>, int>);
static_assert(std::is_same_v<edt::WorstReference_t<int&, int&&>, int&>);
static_assert(std::is_same_v<edt::WorstReference_t<int&&, int&&>, int&&>);
static_assert(std::is_same_v<edt::WorstReference_t<const int, int&, int&&>, const int>);
static_assert(std::is_same_v<edt::WorstReference_t<const int, volatile int&, int&&>, const int>);
static_assert(std::is_same_v<edt::WorstReference_t<volatile int, const int&, int&&>, volatile int>);
static_assert(std::is_same_v<edt::WorstReference_t<const int&&, volatile int&&>, const int&&>);
