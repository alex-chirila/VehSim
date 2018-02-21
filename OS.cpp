#include "OS.h"

using namespace std;

TimeTracker::TimeTracker()
{
    currTick = 0;
}

void TimeTracker::Tick()
{
    currTick = currTick + 1;
}

uint64_t TimeTracker::getTick()
{
    return currTick;
}

void TimeTracker::MainTask()
{
    //function to basically do clerical stuff, print debug statements etc

}
