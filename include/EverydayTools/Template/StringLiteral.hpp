#pragma once

#include <algorithm>
#include <string_view>

namespace edt {
template <size_t N>
struct StringLiteral {
  constexpr StringLiteral(const char (&str)[N]) { std::copy_n(str, N, value); }

  [[nodiscard]] inline constexpr std::string_view GetView() const {
    return value;
  }

  char value[N];
};
}  // namespace edt