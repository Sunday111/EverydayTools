#pragma once

#include <type_traits>

#include "../ReflectTypeInterface/ClassReflectTypeInterface.hpp"
#include "TypeFlagTraits.hpp"


namespace edt::reflection::detail {
template <>
struct TypeFlagTraits<TypeFlags::Class> {
  template <typename T>
  static inline constexpr bool IsTypeApplicable = std::is_class_v<T>;
  using Container = ClassReflectTypeInterface;

  static void ConstructContainer(Container&) {}
};
}  // namespace edt::reflection::detail
