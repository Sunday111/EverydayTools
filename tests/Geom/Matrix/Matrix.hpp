#include <cmath>

#include "EverydayTools/Geom/Matrix.hpp"
#include "EverydayTools/UnusedVar.hpp"
#include "gtest/gtest.h"

using namespace edt::geom;

template <typename T>
bool Equal(const T& a, const T& b) {
  if constexpr (std::is_integral_v<T>) {
    return a == b;
  } else {
    return std::abs(static_cast<T>(a - b)) < T(0.00001f);
  }
}

template <typename T, size_t R, size_t C>
struct SkippedTest {
  static void Exec() {}
};

template <typename T>
struct TupleTraits {};

template <typename Head_, typename... Tail_>
struct TupleTraits<std::tuple<Head_, Tail_...>> {
  using Head = Head_;
  using Tail = std::tuple<Tail_...>;
};

template <typename T>
struct IndexSequenceTraits {};

template <size_t Head_, size_t... Tail_>
struct IndexSequenceTraits<std::index_sequence<Head_, Tail_...>> {
  static constexpr auto Head = Head_;
  using Tail = std::index_sequence<Tail_...>;
};

template <typename T>
struct HasNext {};

template <typename... Types>
struct HasNext<std::tuple<Types...>> {
  static constexpr bool Value = sizeof...(Types) > 1;
};

template <size_t... Values>
struct HasNext<std::index_sequence<Values...>> {
  static constexpr bool Value = sizeof...(Values) > 1;
};

template <typename T>
constexpr bool HasNext_v = HasNext<T>::Value;

template <typename TypesTuple, typename RowsValues, typename ColumnValues,
          typename StartRowValues = RowsValues,
          typename StartColumnValues = ColumnValues>
struct TemplateIterator {
  static constexpr bool HasNext =
      HasNext_v<TypesTuple> || HasNext_v<RowsValues> || HasNext_v<ColumnValues>;

  using TypesTraits = TupleTraits<TypesTuple>;
  using RowsTraits = IndexSequenceTraits<RowsValues>;
  using ColumnsTraits = IndexSequenceTraits<ColumnValues>;

  using HeadType = typename TypesTraits::Head;
  static constexpr auto HeadRow = RowsTraits::Head;
  static constexpr auto HeadColumn = ColumnsTraits::Head;

  using TypesTail = typename TypesTraits::Tail;
  using RowsTail = typename RowsTraits::Tail;
  using ColumnsTail = typename ColumnsTraits::Tail;

  using IncrementType = std::conditional_t<
      HasNext_v<TypesTuple>,
      TemplateIterator<TypesTail, StartRowValues, StartColumnValues>, void>;

  using IncrementRow = std::conditional_t<
      HasNext_v<RowsValues>,
      TemplateIterator<TypesTuple, RowsTail, StartColumnValues, StartRowValues>,
      IncrementType>;

  using IncrementColumn =
      std::conditional_t<HasNext_v<ColumnValues>,
                         TemplateIterator<TypesTuple, RowsValues, ColumnsTail,
                                          StartRowValues, StartColumnValues>,
                         IncrementRow>;

  using Next = IncrementColumn;
};

template <template <typename, size_t, size_t> typename Target,
          typename Iterator>
struct CombineHelper {
  static void Execute() {
    using T = typename Iterator::HeadType;
    constexpr auto R = Iterator::HeadRow;
    constexpr auto C = Iterator::HeadColumn;
    using Final = Target<T, R, C>;
    Final::Execute();
    if constexpr (Iterator::HasNext) {
      using NextIterator = typename Iterator::Next;
      CombineHelper<Target, NextIterator>::Execute();
    }
  }
};

template <template <typename, size_t, size_t> typename Target, size_t Rows,
          size_t Columns, typename... Types>
struct Combinator {
  static void Execute() {
    using TypesTuple = std::tuple<Types...>;
    using BaseRowsSequence = decltype(std::make_index_sequence<Rows + 1>());
    using RowsSequence = typename IndexSequenceTraits<BaseRowsSequence>::Tail;
    using BaseColumnsSequence =
        decltype(std::make_index_sequence<Columns + 1>());
    using ColumnsSequence =
        typename IndexSequenceTraits<BaseColumnsSequence>::Tail;
    using Iterator =
        TemplateIterator<TypesTuple, RowsSequence, ColumnsSequence>;
    using Helper = CombineHelper<Target, Iterator>;
    Helper::Execute();
  }
};

template <typename T>
struct Randomizer {
  static void Randomize(T& value) { value = static_cast<T>(std::rand() % 256); }
};

template <typename T, size_t R, size_t C>
struct Randomizer<Matrix<T, R, C>> {
  static void Randomize(Matrix<T, R, C>& value) {
    for (size_t i = 0; i < R; ++i) {
      for (size_t j = 0; j < C; ++j) {
        Randomizer<T>::Randomize(value.At(i, j));
      }
    }
  }
};

template <typename T>
void RandomizeValue(T& value) {
  Randomizer<std::decay_t<T>>::Randomize(value);
}

template <bool, typename Type>
struct InputGenerator {};

template <bool random, typename... Types>
struct InputGenerator<random, std::tuple<Types...>> {
  using Result = std::tuple<Types...>;

  template <size_t... Indices>
  static void Randomize(std::index_sequence<Indices...>, Result& result) {
    UnusedVar(result);  // <- 0 Arguments case
    (RandomizeValue(std::get<Indices>(result)), ...);
  }

  static Result Generate() {
    Result result;
    if constexpr (random) {
      Randomize(std::index_sequence_for<Types...>(), result);
    }
    return result;
  }
};

template <typename T, size_t R, size_t C,
          template <typename, size_t, size_t> typename Target>
struct TestArgumentCombinator {
  using FT = Target<T, R, C>;
  using Input = typename FT::Input;

  template <size_t... Indices>
  static void FoldExecuteArgs(Input input, std::index_sequence<Indices...>) {
    UnusedVar(input);  // <- 0 Arguments case
    FT::Execute(std::get<Indices>(input)...);
  }

  static void Execute() {
    if constexpr (FT::Enabled) {
      using Generator = InputGenerator<FT::RandomInput, Input>;
      FoldExecuteArgs(Generator::Generate(),
                      std::make_index_sequence<std::tuple_size_v<Input>>());
    }
  }
};

template <template <typename, size_t, size_t> typename Target>
struct TestCaller {
  template <typename T, size_t R, size_t C>
  using Adjusted = TestArgumentCombinator<T, R, C, Target>;

  static void Execute() {
    using FinalCombinator =
        Combinator<Adjusted, 6, 6, unsigned int, int, float, double>;
    FinalCombinator::Execute();
  }
};

template <template <typename, size_t, size_t> typename Target>
void RunTest() {
  TestCaller<Target>::Execute();
}
