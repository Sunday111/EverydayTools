#pragma once

#include "../Template/TypeHasMethod.hpp"
#include "Detail/ReflectType.hpp"
#include "Detail/TypeInfoContainer.hpp"


namespace edt::reflection {
ImplementHasMethod(GetTypeFlags);

template <typename T>
constexpr TypeFlags SelectTypeFlags() {
  if constexpr (HasGetTypeFlagsMethod_v<T>) {
    return T::GetTypeFlags();
  } else {
    return detail::GetTypeFlags<T>();
  }
}

template <typename T>
class TypeInfo : public detail::TypeInfoContainer<SelectTypeFlags<T>()> {
 public:
  TypeInfo() {
    Construct(detail::BitsetValuesList<GetTypeFlags()>{});
    detail::CallReflectType(*this);
  }

 protected:
  template <auto... typeFlag>
  void Construct(edt::ValuesList<typeFlag...>) {
    (detail::TypeFlagTraits<typeFlag>::template ConstructContainer<T>(*this),
     ...);
  }
};
}  // namespace edt::reflection
