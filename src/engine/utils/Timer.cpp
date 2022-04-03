#include "Timer.h"

namespace Run {
    void Timer::start() {
        startTime = HighResClock::now();
    }

	void Timer::end() {
        endTime = HighResClock::now();
        duration = endTime - startTime;
        miliseconds = duration.count() * 1000.0f;
        seconds = duration.count();
    }
}