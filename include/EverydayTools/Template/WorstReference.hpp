#pragma once

#include "WorstQualifiers.hpp"

namespace edt {
template <typename Head, typename... Tail>
class WorstReference {
  using WQ = WorstQualifiers_t<Head, Tail...>;
  static constexpr bool rValueReference =
      allTypesMatchCondition<std::is_rvalue_reference, Head, Tail...>;
  static constexpr bool lValueReference =
      allTypesMatchCondition<std::is_lvalue_reference, Head, Tail...>;
  using LValueApplied =
      ApplyIf<lValueReference, std::add_lvalue_reference_t, WQ>;

 public:
  using type =
      ApplyIf<rValueReference, std::add_rvalue_reference_t, LValueApplied>;
};

template <typename... Types>
using WorstReference_t = typename WorstReference<Types...>::type;
}  // namespace edt