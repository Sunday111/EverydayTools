#pragma once

#include "Operators/MakeCommonOperators.hpp"

namespace edt::reflection {
class CommonTypeInfo {
 public:
  std::size_t size;
  CommonOperators operators;
};
}  // namespace edt::reflection