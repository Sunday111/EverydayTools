#include "EverydayTools/Template/ValuesList.h"
#include "EverydayTools/Template/TypesList.h"

using namespace edt;

namespace values_list_concat
{
    namespace A
    {
        using Arg1 = ValuesList<>;
        using Arg2 = ValuesList<>;
        using Expected = ValuesList<>;
        using Actual = ValuesList_ConcatT<Arg1, Arg2>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace B
    {
        using Arg1 = ValuesList<1>;
        using Arg2 = ValuesList<>;
        using Expected = ValuesList<1>;
        using Actual = ValuesList_ConcatT<Arg1, Arg2>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace C
    {
        using Arg1 = ValuesList<>;
        using Arg2 = ValuesList<1>;
        using Expected = ValuesList<1>;
        using Actual = ValuesList_ConcatT<Arg1, Arg2>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace D
    {
        using Arg1 = ValuesList<2>;
        using Arg2 = ValuesList<1>;
        using Expected = ValuesList<2, 1>;
        using Actual = ValuesList_ConcatT<Arg1, Arg2>;
        static_assert(std::is_same_v<Expected, Actual>);
    }
}

namespace values_list_map_to_type
{
    template<auto Value>
    using F = std::conditional_t<(Value > 0), std::true_type, std::false_type>;

    namespace A
    {
        using Arg = ValuesList<>;
        using Expected = TypesList<>;
        using Actual = ValuesList_MapToTypeT<F, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace B
    {
        using Arg = ValuesList<-1>;
        using Expected = TypesList<std::false_type>;
        using Actual = ValuesList_MapToTypeT<F, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace C
    {
        using Arg = ValuesList<1>;
        using Expected = TypesList<std::true_type>;
        using Actual = ValuesList_MapToTypeT<F, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace D
    {
        using Arg = ValuesList<-1, 1>;
        using Expected = TypesList<std::false_type, std::true_type>;
        using Actual = ValuesList_MapToTypeT<F, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }
}

namespace values_list_map_to_value
{
    template<auto Arg>
    struct F
    {
        static inline constexpr auto Value = Arg * Arg;
    };

    namespace A
    {
        using Arg = ValuesList<>;
        using Expected = ValuesList<>;
        using Actual = ValuesList_MapToValueT<F, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace B
    {
        using Arg = ValuesList<2>;
        using Expected = ValuesList<4>;
        using Actual = ValuesList_MapToValueT<F, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace C
    {
        using Arg = ValuesList<2, 4>;
        using Expected = ValuesList<4, 16>;
        using Actual = ValuesList_MapToValueT<F, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }
}

namespace values_list_filter
{
    template<auto Arg>
    struct Predicate
    {
        static inline constexpr auto Value = Arg > 0;
    };

    namespace A
    {
        using Arg = ValuesList<>;
        using Expected = ValuesList<>;
        using Actual = ValuesList_FilterT<Predicate, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace B
    {
        using Arg = ValuesList<-1>;
        using Expected = ValuesList<>;
        using Actual = ValuesList_FilterT<Predicate, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace C
    {
        using Arg = ValuesList<1>;
        using Expected = ValuesList<1>;
        using Actual = ValuesList_FilterT<Predicate, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace D
    {
        using Arg = ValuesList<-1, 1>;
        using Expected = ValuesList<1>;
        using Actual = ValuesList_FilterT<Predicate, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }
}

namespace values_list_to_integer_sequence
{
    namespace A
    {
        using Arg = ValuesList<1, 2, 3>;
        using Expected = std::integer_sequence<int, 1, 2, 3>;
        using Actual = ValuesList_ToIntegerSequenceT<int, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }
}

namespace values_list_from_integer_sequence
{
    namespace A
    {
        using Arg = std::integer_sequence<int, 1, 2, 3>;
        using Expected = ValuesList<1, 2, 3>;
        using Actual = ValuesList_FromIntegerSequenceT<Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }
}
