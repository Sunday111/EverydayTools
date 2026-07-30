#include "edt/template/values_list.hpp"

#include "edt/template/types_list.hpp"

namespace values_list_concat
{
namespace A
{
using Arg1 = edt::ValuesList<>;
using Arg2 = edt::ValuesList<>;
using Expected = edt::ValuesList<>;
using Actual = edt::ValuesList_ConcatT<Arg1, Arg2>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace A

namespace B
{
using Arg1 = edt::ValuesList<1>;
using Arg2 = edt::ValuesList<>;
using Expected = edt::ValuesList<1>;
using Actual = edt::ValuesList_ConcatT<Arg1, Arg2>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace B

namespace C
{
using Arg1 = edt::ValuesList<>;
using Arg2 = edt::ValuesList<1>;
using Expected = edt::ValuesList<1>;
using Actual = edt::ValuesList_ConcatT<Arg1, Arg2>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace C

namespace D
{
using Arg1 = edt::ValuesList<2>;
using Arg2 = edt::ValuesList<1>;
using Expected = edt::ValuesList<2, 1>;
using Actual = edt::ValuesList_ConcatT<Arg1, Arg2>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace D
}  // namespace values_list_concat

namespace values_list_map_to_type
{
template <auto Value>
using F = std::conditional_t<(Value > 0), std::true_type, std::false_type>;

namespace A
{
using Arg = edt::ValuesList<>;
using Expected = edt::TypesList<>;
using Actual = edt::ValuesList_MapToTypeT<F, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace A

namespace B
{
using Arg = edt::ValuesList<-1>;
using Expected = edt::TypesList<std::false_type>;
using Actual = edt::ValuesList_MapToTypeT<F, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace B

namespace C
{
using Arg = edt::ValuesList<1>;
using Expected = edt::TypesList<std::true_type>;
using Actual = edt::ValuesList_MapToTypeT<F, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace C

namespace D
{
using Arg = edt::ValuesList<-1, 1>;
using Expected = edt::TypesList<std::false_type, std::true_type>;
using Actual = edt::ValuesList_MapToTypeT<F, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace D
}  // namespace values_list_map_to_type

namespace values_list_map_to_value
{
template <auto Arg>
struct F
{
    static inline constexpr auto Value = Arg * Arg;
};

namespace A
{
using Arg = edt::ValuesList<>;
using Expected = edt::ValuesList<>;
using Actual = edt::ValuesList_MapToValueT<F, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace A

namespace B
{
using Arg = edt::ValuesList<2>;
using Expected = edt::ValuesList<4>;
using Actual = edt::ValuesList_MapToValueT<F, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace B

namespace C
{
using Arg = edt::ValuesList<2, 4>;
using Expected = edt::ValuesList<4, 16>;
using Actual = edt::ValuesList_MapToValueT<F, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace C
}  // namespace values_list_map_to_value

namespace values_list_filter
{
template <auto Arg>
struct Predicate
{
    static inline constexpr auto Value = Arg > 0;
};

namespace A
{
using Arg = edt::ValuesList<>;
using Expected = edt::ValuesList<>;
using Actual = edt::ValuesList_FilterT<Predicate, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace A

namespace B
{
using Arg = edt::ValuesList<-1>;
using Expected = edt::ValuesList<>;
using Actual = edt::ValuesList_FilterT<Predicate, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace B

namespace C
{
using Arg = edt::ValuesList<1>;
using Expected = edt::ValuesList<1>;
using Actual = edt::ValuesList_FilterT<Predicate, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace C

namespace D
{
using Arg = edt::ValuesList<-1, 1>;
using Expected = edt::ValuesList<1>;
using Actual = edt::ValuesList_FilterT<Predicate, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace D
}  // namespace values_list_filter

namespace values_list_to_integer_sequence::A
{
using Arg = edt::ValuesList<1, 2, 3>;
using Expected = std::integer_sequence<int, 1, 2, 3>;
using Actual = edt::ValuesList_ToIntegerSequenceT<int, Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace values_list_to_integer_sequence::A

namespace values_list_from_integer_sequence::A
{
using Arg = std::integer_sequence<int, 1, 2, 3>;
using Expected = edt::ValuesList<1, 2, 3>;
using Actual = edt::ValuesList_FromIntegerSequenceT<Arg>;
static_assert(std::is_same_v<Expected, Actual>);
}  // namespace values_list_from_integer_sequence::A
