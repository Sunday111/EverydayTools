#pragma once

#include "../../ITypeInfo.hpp"

namespace edt::reflection::detail {
struct InvalidTypeTrait {};

template <TypeFlags flag>
struct TypeFlagTraits : InvalidTypeTrait {};

template <auto flag>
struct TypeFlagHasTrait {
  static constexpr bool Value =
      !std::is_base_of_v<InvalidTypeTrait, TypeFlagTraits<flag>>;
};

template <typename T>
struct FlagIsDefaultApplicableFilter {
  template <auto flag>
  struct Predicate {
    static constexpr bool Value =
        TypeFlagTraits<flag>::template IsTypeApplicable<T>;
  };
};
}  // namespace edt::reflection::detail