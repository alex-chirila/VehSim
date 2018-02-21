///Todo and questions:
///1. Can multiple vehicles occupy the same tile? If so, how many?
    ///A. YES They can!
    ///Todo: add code to restrict vehicles based on tile difficulty
///2. Raise exceptions if there are errors in number of tiles?
///3. Should there be a lower limit to number of tiles? (eg 3)
///4. Add vehicle fuel levels?
///6. Check if vehicle idle too long (e.g. at a traffic light gone awry)
///7. Overhaul init: section data, use only 1 file
///8. Path to ini file given as CLI param

#include <iostream>
#include "Init.h"
#include "OS.h"

using namespace std;

int main()
{
    //new sim!
    Initializer startRoutine; //constructor call
    DispatchManager DM; //constructor call
    RoadManager RM; //constructor call

    ///TODO: Overhaul calls
    //startRoutine.PopulateSectionList(); not needed, done in constructor
    startRoutine.ParseIniData(DM, RM);
    //startRoutine.readRoadParams(RM); //reads data and passes it to the Road Manager
    //startRoutine.readVehData(DM); //reads data and passes it to the Dispatch Manager

    TimeTracker TT;
    DM.ListVehicles();
    RM.ListTiles();//for debugging purposes..

    while(1)
    {
        TT.MainTask();
        DM.MainTask(TT, RM);
        RM.MainTask(TT);
        TT.Tick();
        if (TT.getTick() > 200)
        {
            cout << "The end." << endl << endl;
            break;
        }
    }
}
