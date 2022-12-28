#pragma once

#include <type_traits>

#include "CommonOperators.hpp"


namespace edt::reflection::detail {
namespace has_operator {
template <typename T, typename = void>
struct has_equals_operator : std::false_type {};

template <typename T>
struct has_equals_operator<T, decltype(void(std::declval<const T&>() ==
                                            std::declval<const T&>()))>
    : std::true_type {};

template <typename T>
inline constexpr bool has_equals_operator_v = has_equals_operator<T>::value;
}  // namespace has_operator

template <typename T>
constexpr CommonOperators::DefaultConstructor
MakeDefaultConstructor() noexcept {
  if constexpr (!std::is_default_constructible_v<T>) {
    return nullptr;
  } else {
    return [](void* object) { new (object) T(); };
  }
}

template <typename T>
constexpr CommonOperators::CopyConstructor MakeCopyConstructor() noexcept {
  if constexpr (!std::is_copy_constructible_v<T>) {
    return nullptr;
  } else {
    return [](void* objectToConstruct, const void* referenceObject) {
      new (objectToConstruct) T(*reinterpret_cast<const T*>(referenceObject));
    };
  }
}

template <typename T>
constexpr CommonOperators::MoveConstructor MakeMoveConstructor() noexcept {
  if constexpr (!std::is_move_constructible_v<T>) {
    return nullptr;
  } else {
    return [](void* destinationObject, void* sourceObject) {
      new (destinationObject) T(std::move(*reinterpret_cast<T*>(sourceObject)));
    };
  }
}

template <typename T>
constexpr CommonOperators::CopyAssign MakeCopyAssignOperator() noexcept {
  if constexpr (!std::is_copy_assignable_v<T>) {
    return nullptr;
  } else {
    return [](void* destination, const void* source) {
      const T& src = *reinterpret_cast<const T*>(source);
      T& dst = *reinterpret_cast<T*>(destination);
      dst = src;
    };
  }
}

template <typename T>
constexpr CommonOperators::MoveAssign MakeMoveAssignOperator() noexcept {
  if constexpr (!std::is_move_assignable_v<T>) {
    return nullptr;
  } else {
    return [](void* destination, void* source) {
      T& src = *reinterpret_cast<T*>(source);
      T& dst = *reinterpret_cast<T*>(destination);
      dst = std::move(src);
    };
  }
}

template <typename T>
constexpr CommonOperators::Destructor MakeDestructor() noexcept {
  if constexpr (!std::is_destructible_v<T>) {
    return nullptr;
  } else {
    return [](void* object) {
      T& obj = *reinterpret_cast<T*>(object);
      obj.~T();
    };
  }
}

template <typename T>
constexpr CommonOperators::Equals MakeEqualsOperator() noexcept {
  if constexpr (!has_operator::has_equals_operator_v<T>) {
    return nullptr;
  } else {
    return [](const void* lhs, const void* rhs) {
      auto& l = *reinterpret_cast<const T*>(lhs);
      auto& r = *reinterpret_cast<const T*>(rhs);
      return l == r;
    };
  }
}

template <typename T>
constexpr CommonOperators MakeCommonOperators() {
  return CommonOperators{
      MakeDefaultConstructor<T>(), MakeCopyConstructor<T>(),
      MakeMoveConstructor<T>(),    MakeCopyAssignOperator<T>(),
      MakeMoveAssignOperator<T>(), MakeEqualsOperator<T>(),
      MakeDestructor<T>()};
}
}  // namespace edt::reflection::detail
