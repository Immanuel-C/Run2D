#include "Timer.h"

namespace Run {
    Timer::Timer(const std::string& name, int flags) : m_name{ name }, m_flags{ flags } {
        startTime = HighResClock::now();
    }


	Timer::~Timer() {
        endTime = HighResClock::now();
        duration = endTime - startTime;
        seconds = duration.count();
        miliseconds = seconds * 1000.0f;
        microsecods = miliseconds * 1000.0f;

        if (m_flags & DONT_PRINT_TIME_DATA)
            return; 

        I_LOG_TRACE("%s took %f seconds, %f miliseconds and %f microseconds", m_name.c_str(), seconds, miliseconds, microsecods);
    }
}