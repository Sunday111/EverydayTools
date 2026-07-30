#pragma once

#include <chrono>
#include <concepts>
#include <functional>
#include <tuple>
#include <utility>

namespace edt
{

template <typename DurationType = std::chrono::nanoseconds, typename F, typename... Args>
    requires std::invocable<F, Args...>
auto MeasureTime(F&& f, Args&&... args)
{
    using Clock = std::chrono::steady_clock;
    const auto t0 = Clock::now();
    const auto elapsed = [&t0]
    {
        return std::chrono::duration_cast<DurationType>(Clock::now() - t0);
    };

    if constexpr (std::same_as<void, std::invoke_result_t<F, Args...>>)
    {
        std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
        return elapsed();
    }
    else
    {
        auto result = std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
        return std::tuple{elapsed(), std::move(result)};
    }
}

template <typename DurationType = std::chrono::nanoseconds, typename F, typename... Args>
    requires std::invocable<F, Args...>
auto MeasureTime_AlwaysReturn(F&& f, Args&&... args)
{
    if constexpr (std::same_as<void, std::invoke_result_t<F, Args...>>)
    {
        return std::tuple{MeasureTime<DurationType>(std::forward<F>(f), std::forward<Args>(args)...)};
    }
    else
    {
        return MeasureTime<DurationType>(std::forward<F>(f), std::forward<Args>(args)...);
    }
}

}  // namespace edt
