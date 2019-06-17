#include "EverydayTools/Template/TypesList.h"
#include "EverydayTools/Template/ValuesList.h"

using namespace edt;

namespace types_list_concat_test
{
    namespace A
    {
        using Arg_1 = TypesList<>;
        using Arg_2 = TypesList<>;
        using Expected = TypesList<>;
        using Actual = TypesList_ConcatT<Arg_1, Arg_2>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace B
    {
        using Arg_1 = TypesList<int>;
        using Arg_2 = TypesList<>;
        using Expected = TypesList<int>;
        using Actual = TypesList_ConcatT<Arg_1, Arg_2>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace C
    {
        using Arg_1 = TypesList<>;
        using Arg_2 = TypesList<int>;
        using Expected = TypesList<int>;
        using Actual = TypesList_ConcatT<Arg_1, Arg_2>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace D
    {
        using Arg_1 = TypesList<float>;
        using Arg_2 = TypesList<int>;
        using Expected = TypesList<float, int>;
        using Actual = TypesList_ConcatT<Arg_1, Arg_2>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace E
    {
        using Arg_1 = TypesList<int>;
        using Arg_2 = TypesList<int>;
        using Expected = TypesList<int, int>;
        using Actual = TypesList_ConcatT<Arg_1, Arg_2>;
        static_assert(std::is_same_v<Expected, Actual>);
    }
}

namespace types_list_map_to_type_test
{
    template<typename T>
    using F = std::decay_t<T>;

    namespace A
    {
        using Arg = TypesList<>;
        using Expected = TypesList<>;
        using Actual = TypesList_MapToTypeT<F, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace B
    {
        using Arg = TypesList<int&&, float&&>;
        using Expected = TypesList<int, float>;
        using Actual = TypesList_MapToTypeT<F, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }
}

namespace types_list_map_to_value_test
{
    template<typename T>
    struct F
    {
        static constexpr bool Value = std::is_integral_v<T>;
    };

    namespace A
    {
        using Arg = TypesList<>;
        using Expected = ValuesList<>;
        using Actual = TypesList_MapToValueT<F, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace B
    {
        using Arg = TypesList<int>;
        using Expected = ValuesList<true>;
        using Actual = TypesList_MapToValueT<F, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace C
    {
        using Arg = TypesList<int, float>;
        using Expected = ValuesList<true, false>;
        using Actual = TypesList_MapToValueT<F, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }
}

namespace types_list_filter_test
{
    template<typename T>
    struct Predicate
    {
        static constexpr bool Value = std::is_integral_v<T>;
    };

    namespace A
    {
        using Arg = TypesList<>;
        using Expected = TypesList<>;
        using Actual = TypesList_FilterT<Predicate, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace B
    {
        using Arg = TypesList<int>;
        using Expected = TypesList<int>;
        using Actual = TypesList_FilterT<Predicate, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace C
    {
        using Arg = TypesList<int, float, double>;
        using Expected = TypesList<int>;
        using Actual = TypesList_FilterT<Predicate, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }

    namespace D
    {
        using Arg = TypesList<float, double>;
        using Expected = TypesList<>;
        using Actual = TypesList_FilterT<Predicate, Arg>;
        static_assert(std::is_same_v<Expected, Actual>);
    }
}
