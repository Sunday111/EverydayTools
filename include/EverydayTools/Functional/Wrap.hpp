#pragma once

namespace edt::details {
template <typename Target, typename Wrapper>
class WrappedFunction {
 public:
  WrappedFunction(Target t, Wrapper w)
      : target(std::move(t)), wrapper(std::move(w)) {}

  template <typename... Args>
  decltype(auto) operator()(Args&&... args) const {
    using TargetReturnType = decltype(target(std::forward<Args>(args)...));
    return wrapper(
        static_cast<TargetReturnType>(target(std::forward<Args>(args)...)));
  }

  Target target;
  Wrapper wrapper;
};
}  // namespace edt::details

namespace edt {
template <typename Target, typename Wrapper>
decltype(auto) WrapFunction(Target&& target, Wrapper&& wrapper) {
  using namespace edt::details;
  return WrappedFunction{std::forward<Target>(target),
                         std::forward<Wrapper>(wrapper)};
}
}  // namespace edt
