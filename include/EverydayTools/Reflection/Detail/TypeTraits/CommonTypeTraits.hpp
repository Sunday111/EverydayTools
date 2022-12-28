#pragma once

#include "../ReflectTypeInterface/CommonReflectTypeInterface.hpp"
#include "TypeFlagTraits.hpp"


namespace edt::reflection::detail {
template <>
struct TypeFlagTraits<TypeFlags::Common> {
  template <typename T>
  static inline constexpr bool IsTypeApplicable = true;

  using Container = CommonReflectTypeInterface;

  template <typename T>
  static void ConstructContainer(Container& c) {
    c.size = sizeof(T);
    c.operators = detail::MakeCommonOperators<T>();
  }
};
}  // namespace edt::reflection::detail
