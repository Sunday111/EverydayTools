#include "edt/template/types_list.hpp"

#include "edt/template/values_list.hpp"

namespace types_list_concat_test
{
namespace A
{
using Arg_1 = edt::TypesList<>;
using Arg_2 = edt::TypesList<>;
using Expected = edt::TypesList<>;
using Actual = edt::TypesList_ConcatT<Arg_1, Arg_2>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace A

namespace B
{
using Arg_1 = edt::TypesList<int>;
using Arg_2 = edt::TypesList<>;
using Expected = edt::TypesList<int>;
using Actual = edt::TypesList_ConcatT<Arg_1, Arg_2>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace B

namespace C
{
using Arg_1 = edt::TypesList<>;
using Arg_2 = edt::TypesList<int>;
using Expected = edt::TypesList<int>;
using Actual = edt::TypesList_ConcatT<Arg_1, Arg_2>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace C

namespace D
{
using Arg_1 = edt::TypesList<float>;
using Arg_2 = edt::TypesList<int>;
using Expected = edt::TypesList<float, int>;
using Actual = edt::TypesList_ConcatT<Arg_1, Arg_2>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace D

namespace E
{
using Arg_1 = edt::TypesList<int>;
using Arg_2 = edt::TypesList<int>;
using Expected = edt::TypesList<int, int>;
using Actual = edt::TypesList_ConcatT<Arg_1, Arg_2>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace E
}  // namespace types_list_concat_test

namespace types_list_map_to_type_test
{
template <typename T>
using F = std::decay_t<T>;

namespace A
{
using Arg = edt::TypesList<>;
using Expected = edt::TypesList<>;
using Actual = edt::TypesList_MapToTypeT<F, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace A

namespace B
{
using Arg = edt::TypesList<int&&, float&&>;
using Expected = edt::TypesList<int, float>;
using Actual = edt::TypesList_MapToTypeT<F, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace B
}  // namespace types_list_map_to_type_test

namespace types_list_map_to_value_test
{
template <typename T>
struct F
{
    static constexpr bool Value = std::is_integral_v<T>;
};

namespace A
{
using Arg = edt::TypesList<>;
using Expected = edt::ValuesList<>;
using Actual = edt::TypesList_MapToValueT<F, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace A

namespace B
{
using Arg = edt::TypesList<int>;
using Expected = edt::ValuesList<true>;
using Actual = edt::TypesList_MapToValueT<F, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace B

namespace C
{
using Arg = edt::TypesList<int, float>;
using Expected = edt::ValuesList<true, false>;
using Actual = edt::TypesList_MapToValueT<F, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace C
}  // namespace types_list_map_to_value_test

namespace types_list_filter_test
{
template <typename T>
struct Predicate
{
    static constexpr bool Value = std::is_integral_v<T>;
};

namespace A
{
using Arg = edt::TypesList<>;
using Expected = edt::TypesList<>;
using Actual = edt::TypesList_FilterT<Predicate, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace A

namespace B
{
using Arg = edt::TypesList<int>;
using Expected = edt::TypesList<int>;
using Actual = edt::TypesList_FilterT<Predicate, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace B

namespace C
{
using Arg = edt::TypesList<int, float, double>;
using Expected = edt::TypesList<int>;
using Actual = edt::TypesList_FilterT<Predicate, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace C

namespace D
{
using Arg = edt::TypesList<float, double>;
using Expected = edt::TypesList<>;
using Actual = edt::TypesList_FilterT<Predicate, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace D
}  // namespace types_list_filter_test

namespace
{
struct A;
struct B;
struct C;
}  // namespace

static_assert(edt::TypesList_IndexOfV<A, edt::TypesList<A, B, C>> == 0);
static_assert(edt::TypesList_IndexOfV<B, edt::TypesList<A, B, C>> == 1);
static_assert(edt::TypesList_IndexOfV<C, edt::TypesList<A, B, C>> == 2);
static_assert(edt::TypesList_IndexOfV<C, edt::TypesList<C>> == 0);

// Finds the first occurrence.
static_assert(edt::TypesList_IndexOfV<B, edt::TypesList<A, B, B>> == 1);

// Qualifiers are part of the type.
static_assert(edt::TypesList_IndexOfV<const int, edt::TypesList<int, const int>> == 1);
static_assert(edt::TypesList_IndexOfV<int&, edt::TypesList<int, int&&, int&>> == 2);
