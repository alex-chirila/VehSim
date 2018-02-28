#pragma once
#include <cstdint>

class TimeTracker
{
    public:
        TimeTracker();
        uint16_t Tick(); //return value used for error codes
        uint64_t getTick();
        uint64_t getEndTick();
        uint64_t getStartTick();
        void MainTask();
        void setStartTick(uint64_t);
        void setEndTick(uint64_t);
        //void setTick() -- NEVER arbitrarily set tick. thank you.

    private:
        uint64_t currTick;
        uint64_t endTick;
        uint64_t startTick;
};
