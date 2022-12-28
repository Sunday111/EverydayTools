#pragma once

#include <type_traits>

namespace edt::detail {
template <template <typename...> typename Trait, typename... Args>
inline constexpr bool template_and() {
  return (Trait<Args>::value || ...);
}
}  // namespace edt::detail

namespace edt {
template <template <typename...> typename Trait, typename... Args>
struct AnyTypeMatchCondition {
  static constexpr bool value = detail::template_and<Trait, Args...>();
};

template <template <typename...> typename Trait, typename... Args>
inline constexpr bool anyTypeMatchCondition =
    AnyTypeMatchCondition<Trait, Args...>::value;
}  // namespace edt
