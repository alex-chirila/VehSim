///Todo and questions:

///CAT: Todo (core features pertaining to sim)
///3. Should there be a lower limit to number of tiles? (eg 3)

///CAT: Todo (advanced features)
/// Add vehicle fuel levels
/// Check if vehicle idle too long (e.g. at a traffic light gone awry)
/// Path of vehicle given as a series of segments
/// Segment junction list
/// Vehicle line assigned by DM
/// Passenger simulator: while waiting in station, add odds for a passenger to arrive
    ///Direct influence: tile difficulty for a certain vehicle

///CAT: Nice-to-have

/// Can multiple vehicles occupy the same tile? If so, how many?
    /// YES They can!
    /// Todo: add code to restrict vehicles based on tile difficulty
/// Path to ini file given as CLI param

#include <iostream>
#include "Init.h"
#include "OS.h"

//#define DEBUG

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
        DM.MainTask(TT, RM);
        RM.MainTask(TT);
        TT.Tick();
        if (TT.getTick() >= nCyclesToRun)
        {
            cout << "The end. Current tick: " << TT.getTick() << endl << endl;
            break;
        }
    }
}
