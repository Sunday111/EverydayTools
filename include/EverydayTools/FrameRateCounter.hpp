#pragma once

#include <chrono>
#include <vector>

template <typename Period = std::chrono::nanoseconds>
class FrameRateCounter
{
public:
    FrameRateCounter() { SetDesiredFps(60); }

    /* average will ramp up until the buffer is full */
    /* returns average ticks per frame over the MaxSamples last frames */
    Period CalcAverageTick(Period newtick)
    {
        ticksum -= ticklist[tickindex];     /* subtract value falling off */
        ticksum += newtick;                 /* add new value */
        ticklist[tickindex] = newtick;      /* save new value so it can be subtracted later */
        if (++tickindex == ticklist.size()) /* inc buffer index */
            tickindex = 0;

        /* return average */
        return (ticksum / ticklist.size());
    }

    Period GetDesiredFrameDuration() const { return m_desiredFrameDuration; }

    void SetDesiredFps(size_t desiredFPS)
    {
        m_desiredFPS = desiredFPS;
        ticklist.resize(static_cast<size_t>(static_cast<float>(m_desiredFPS) * 1.5));
        tickindex = 0;
        ticksum = Period();
        m_desiredFrameDuration = GetDesiredFrameDuration(m_desiredFPS);
    }

private:
    static Period GetDesiredFrameDuration(size_t desiredFPS)
    {
        using namespace std::chrono;
        return Period(duration_cast<Period>(seconds(1))) / desiredFPS;
    }

    size_t tickindex;
    Period ticksum;
    std::vector<Period> ticklist;
    size_t m_desiredFPS;
    Period m_desiredFrameDuration;
};