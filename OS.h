#pragma once
#include <cstdint>

class TimeTracker
{
    public:
        TimeTracker();
        void Tick();
        uint64_t getTick();
        void MainTask();

    private:
        uint64_t currTick;
};
