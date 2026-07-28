#include "edt/frame_rate_counter.hpp"

#include <chrono>

#include "gtest/gtest.h"

using namespace std::chrono_literals;

TEST(FrameRateCounterTest, DesiredFrameDuration)
{
    edt::FrameRateCounter<std::chrono::nanoseconds> counter;
    EXPECT_EQ(counter.GetDesiredFps(), 60U);
    EXPECT_EQ(counter.GetDesiredFrameDuration(), std::chrono::nanoseconds(1'000'000'000 / 60));

    counter.SetDesiredFps(30);
    EXPECT_EQ(counter.GetDesiredFps(), 30U);
    EXPECT_EQ(counter.GetDesiredFrameDuration(), std::chrono::nanoseconds(1'000'000'000 / 30));
}

TEST(FrameRateCounterTest, AverageRampsUpThenTracksTheWindow)
{
    edt::FrameRateCounter<std::chrono::nanoseconds> counter;
    counter.SetDesiredFps(2);  // window of 3 samples

    // Ramping up: the window still holds zeroes.
    EXPECT_EQ(counter.CalcAverageTick(30ns), 10ns);
    EXPECT_EQ(counter.CalcAverageTick(30ns), 20ns);
    EXPECT_EQ(counter.CalcAverageTick(30ns), 30ns);

    // Full window: the oldest sample falls off.
    EXPECT_EQ(counter.CalcAverageTick(60ns), 40ns);
}

TEST(FrameRateCounterTest, ResettingFpsClearsTheWindow)
{
    edt::FrameRateCounter<std::chrono::nanoseconds> counter;
    counter.SetDesiredFps(2);
    counter.CalcAverageTick(300ns);

    counter.SetDesiredFps(2);
    EXPECT_EQ(counter.CalcAverageTick(30ns), 10ns);
}
