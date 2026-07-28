#include "edt/time/measure_time.hpp"

#include <chrono>
#include <thread>

#include "gtest/gtest.h"

TEST(MeasureTimeTest, ReturnsDurationOnlyForVoidCallables)
{
    const auto elapsed = edt::MeasureTime([] {});
    static_assert(std::same_as<decltype(elapsed), const std::chrono::nanoseconds>);
    EXPECT_GE(elapsed.count(), 0);
}

// Note there is no std::invoke<R> in C++20 to reach for here: the explicit
// argument would bind to the callable's own template parameter, not a return
// type. That is std::invoke_r, and it is C++23.
TEST(MeasureTimeTest, ReturnsDurationAndResultForNonVoidCallables)
{
    const auto [elapsed, result] = edt::MeasureTime([] { return 42; });
    EXPECT_EQ(result, 42);
    EXPECT_GE(elapsed.count(), 0);
}

TEST(MeasureTimeTest, ForwardsArguments)
{
    const auto [elapsed, result] = edt::MeasureTime([](int a, int b) { return a + b; }, 20, 22);
    EXPECT_EQ(result, 42);
    EXPECT_GE(elapsed.count(), 0);
}

TEST(MeasureTimeTest, HonoursTheRequestedDurationType)
{
    const auto elapsed = edt::MeasureTime<std::chrono::microseconds>([] {});
    static_assert(std::same_as<decltype(elapsed), const std::chrono::microseconds>);
    EXPECT_GE(elapsed.count(), 0);
}

TEST(MeasureTimeTest, AlwaysReturnGivesATupleEitherWay)
{
    const auto void_result = edt::MeasureTime_AlwaysReturn([] {});
    static_assert(std::tuple_size_v<decltype(void_result)> == 1);
    EXPECT_GE(std::get<0>(void_result).count(), 0);

    const auto value_result = edt::MeasureTime_AlwaysReturn([] { return 1; });
    static_assert(std::tuple_size_v<decltype(value_result)> == 2);
    EXPECT_EQ(std::get<1>(value_result), 1);
}

TEST(MeasureTimeTest, MeasuresElapsedTime)
{
    constexpr auto sleep_for = std::chrono::milliseconds(5);
    const auto elapsed = edt::MeasureTime<std::chrono::milliseconds>([&] { std::this_thread::sleep_for(sleep_for); });

    // A steady clock never runs backwards, so this cannot be flaky downwards.
    EXPECT_GE(elapsed, std::chrono::milliseconds(1));
}
