#pragma once

#include <type_traits>

#include "../ReflectTypeInterface/EnumerationReflectTypeInterface.hpp"
#include "TypeFlagTraits.hpp"


namespace edt::reflection::detail {
template <>
struct TypeFlagTraits<TypeFlags::Enumeration> {
  template <typename T>
  static inline constexpr bool IsTypeApplicable = std::is_enum_v<T>;
  using Container = EnumerationReflectTypeInterface;

  template <typename T>
  static void ConstructContainer(Container&) {}
};
}  // namespace edt::reflection::detail
