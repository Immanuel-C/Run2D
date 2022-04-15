#pragma once

#include <chrono>
#include <thread>
#include <iostream>

#include <ILog.h>

#include "DynamicExport.h"


namespace Run {
    struct RUN_API Timer
    {
    private:
        using HighResClock = std::chrono::high_resolution_clock;
        bool m_alreadyEnded = false;
        std::string m_name;
        int m_flags = 0;
    public:
        static constexpr int DONT_PRINT_TIME_DATA = 1;

        std::chrono::time_point<std::chrono::steady_clock> startTime, endTime;

        std::chrono::duration<float> duration;
        float miliseconds;
        float seconds;
        float microsecods;

        Timer(const std::string& name, int flags = 0);

        ~Timer();
    };
}