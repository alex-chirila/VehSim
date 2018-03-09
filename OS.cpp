#include "OS.h"
#include <iostream>

using namespace std;

TimeTracker::TimeTracker()
{
    currTick = 0;
    endTick = 86400; //default to one full day, but may be overwritten
}

uint16_t TimeTracker::Tick()
{
    uint16_t retVal;

    if (endTick > currTick)
    {
        currTick = currTick + 1;
        retVal = 0;
    }
    else
    {
        retVal = 1;
    }

    return retVal;
}

void TimeTracker::setStartTick(uint64_t stTick)
{
    startTick = stTick;
    currTick = stTick;
}

void TimeTracker::setEndTick(uint64_t maxC)
{
    endTick = maxC;
}

uint64_t TimeTracker::getEndTick()
{
    return endTick;
}

uint64_t TimeTracker::getTick()
{
    return currTick;
}

uint64_t TimeTracker::getStartTick()
{
    return startTick;
}

void TimeTracker::MainTask()
{
    //function to basically do clerical stuff, print debug statements etc

}
