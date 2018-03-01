#include <iostream>
#include "Init.h"
#include "OS.h"
#include "Tools.h"

using namespace std;

int main()
{
    uint64_t nCyclesToRun;
    //new sim!
    Initializer startRoutine; //constructor call
    DispatchManager DM; //constructor call
    RoadManager RM; //constructor call
    TimeTracker TT;

    //DM and RM exist, so the Initializer can call them
    startRoutine.ParseIniData(DM, RM, TT);

    nCyclesToRun = TT.getEndTick(); //getTick will return start time, at this point

        #ifdef INIDEBUG
        cout << "The sim is starting at tick: " << TT.getTick() << endl;
        cout << "\tand will end at tick: " << nCyclesToRun << endl << endl;
        #endif // INIDEBUG

    while(1)
    {
        TT.MainTask();
        //DM.MainTask(TT, RM);
        //RM.MainTask(TT);
        TT.Tick();
        if (TT.getTick() >= nCyclesToRun)
        {
            cout << "The end. Current tick: " << TT.getTick() << endl << endl;
            break;
        }
    }
}
