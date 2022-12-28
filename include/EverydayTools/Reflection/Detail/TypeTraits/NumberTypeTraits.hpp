#pragma once

#include <type_traits>

#include "../ReflectTypeInterface/NumberReflectTypeInterface.hpp"
#include "TypeFlagTraits.hpp"


namespace edt::reflection::detail {
template <>
struct TypeFlagTraits<TypeFlags::Number> {
  template <typename T>
  static inline constexpr bool IsTypeApplicable = std::is_arithmetic_v<T>;
  using Container = NumberReflectTypeInterface;

  template <typename T>
  static void ConstructContainer(Container& c) {
    if constexpr (std::is_floating_point_v<T>) {
      c.numberType = NumberType::FloatingPoint;
    } else {
      if constexpr (std::is_signed_v<T>) {
        c.numberType = NumberType::SignedInteger;
      } else {
        c.numberType = NumberType::UnsignedInteger;
      }
    }
  }
};
}  // namespace edt::reflection::detail
