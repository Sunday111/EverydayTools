#pragma once

#include <chrono>
#include <cstddef>
#include <vector>

namespace edt
{
template <typename Duration = std::chrono::nanoseconds>
class FrameRateCounter
{
public:
    FrameRateCounter() { SetDesiredFps(60); }

    Duration CalcAverageTick(Duration new_tick)
    {
        tick_sum_ -= ticks_[tick_index_];
        tick_sum_ += new_tick;
        ticks_[tick_index_] = new_tick;
        if (++tick_index_ == ticks_.size()) tick_index_ = 0;

        return tick_sum_ / ticks_.size();
    }

    [[nodiscard]] Duration GetDesiredFrameDuration() const { return desired_frame_duration_; }

    [[nodiscard]] std::size_t GetDesiredFps() const { return desired_fps_; }

    void SetDesiredFps(std::size_t desired_fps)
    {
        desired_fps_ = desired_fps;
        ticks_.assign((desired_fps * 3) / 2, Duration{});
        tick_index_ = 0;
        tick_sum_ = Duration{};
        desired_frame_duration_ = CalcDesiredFrameDuration(desired_fps);
    }

private:
    [[nodiscard]] static Duration CalcDesiredFrameDuration(std::size_t desired_fps)
    {
        return std::chrono::duration_cast<Duration>(std::chrono::seconds(1)) / desired_fps;
    }

    std::size_t tick_index_ = 0;
    Duration tick_sum_{};
    std::vector<Duration> ticks_;
    std::size_t desired_fps_ = 0;
    Duration desired_frame_duration_{};
};
}  // namespace edt
