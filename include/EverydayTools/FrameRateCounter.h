#pragma once

#include <chrono>

template<typename Period = std::chrono::nanoseconds, size_t MaxSamples = 100>
class FrameRateCounter
{
public:
    /* average will ramp up until the buffer is full */
    /* returns average ticks per frame over the MaxSamples last frames */
    Period CalcAverageTick(Period newtick)
    {
        ticksum -= ticklist[tickindex];  /* subtract value falling off */
        ticksum += newtick;              /* add new value */
        ticklist[tickindex] = newtick;   /* save new value so it can be subtracted later */
        if (++tickindex == MaxSamples)   /* inc buffer index */
            tickindex = 0;

        /* return average */
        return(ticksum / MaxSamples);
    }

    static Period GetDesiredFrameDuration(size_t desiredFPS) {
        using namespace std::chrono;
        return Period(duration_cast<Period>(seconds(1))) / desiredFPS;
    }

    size_t tickindex = 0;
    Period ticksum;
    Period ticklist[MaxSamples]{};
};