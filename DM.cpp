#include <iostream>
#include <cstdint>
#include "DM.h"

using namespace std;

DispatchManager::DispatchManager()
{
    //do nothing
    currVehNo = 0;
    nIdleVehs = 0;
    nDrivers = 0;
}

void DispatchManager::AddVehicle(uint16_t fuel, string vehName)
{
    nIdleVehs++;
    Vehicle tempVeh;
        //Start tick and tile to be later determined!
    tempVeh.sTick = 65535;
    tempVeh.currTile = 65535;
    tempVeh.sTile = 65535;

    tempVeh.fuelLevel = fuel;
    tempVeh.state = STARTIDLE;
    tempVeh.vehID = currVehNo;
    tempVeh.vehEasyName = vehName;
    //todo: add error-handling in case the vehicle cant be added to list
    VehList.push_back(tempVeh);
        #ifdef DEBUG
        cout << "Added vehicle:" << tempVeh.vehEasyName << " with fuel level " << tempVeh.fuelLevel << endl;
        #endif // DEBUG

    currVehNo++;
}
void DispatchManager::AddDriver(string iniName, uint32_t recommendedStart, uint32_t recommendedStop)
{
    Driver tempDrv;

    tempDrv.assignedVehicle = 65535; //default value
    tempDrv.availableStartTime = recommendedStart;
    tempDrv.availableStopTime = recommendedStop;
    tempDrv.driverName = iniName;
    tempDrv.status = NOTASSIGNED;
    tempDrv.todayStartTime = 4294967295;
    tempDrv.todayStopTime = 4294967295;
    DriverList.push_back(tempDrv);
        #ifdef DEBUG
        cout << "Added driver: " << tempDrv.driverName << ", who prefers to start at " << tempDrv.availableStartTime << " and to finish shift at "
            << tempDrv.availableStopTime << endl;
        #endif // DEBUG
    nDrivers++;
}

void DispatchManager::ListVehicles()
{
    for (int i = 0; i < currVehNo; i++)
    {
        cout << "Veh #" << i << ":" << endl;
        cout << "\t current tile: " << VehList[i].currTile << endl;
        cout << "\t set to start on tick: " << VehList[i].sTick << endl;
    }
    cout << "Idling vehicles: " << nIdleVehs << endl;
    cout << endl;
}

void DispatchManager::CheckIfAnyVehicleNeedsToStart(TimeTracker& TT, RoadManager& RM)
{
    if (nIdleVehs > 0)
    {
        uint64_t currTick = TT.getTick();
        for (uint16_t i = 0; i < currVehNo; i++)
        {
            //loop thru all vehicles and check their status
            if ( (STARTIDLE == VehList[i].state) && (currTick == VehList[i].sTick))
            {
                VehList[i].state = MOVING;
                VehList[i].timestampTileEntered = currTick;
                RM.NotifyNewVehicleOnTile(VehList[i].sTile);
                VehList[i].diffOfCurrTile = RM.GetTileDifficulty(VehList[i].sTile);
                nIdleVehs--; //one less vehicle is idling
                cout << "Vehicle " << VehList[i].vehEasyName << " is now moving. Time is: " << currTick << endl;
            } //end if
        } //end for
        //cout << "\t" << nIdleVehs << " remaining to start." << endl;
    }
    else
    {
        //cout << " All vehicles moving at tick " << TT.getTick() << endl;
    }
}
void DispatchManager::MoveVehicles(TimeTracker& TT, RoadManager& RM)
{
    uint64_t currTick = TT.getTick();

    for (uint16_t i = 0; i < currVehNo; i++)
    {
        uint16_t timeSpentOnTile = currTick - VehList[i].timestampTileEntered;

        // Recording tile difficulty at the moment when a vehicle enters the tile
        // allows later changing of the tile difficulty, which will affect only
        // NEW vehicles entering said tile. For example: semaphore tiles.
        if ((MOVING == VehList[i].state) && (timeSpentOnTile == VehList[i].diffOfCurrTile))
        {
            //notify RM to remove vehicle from old tile
            RM.NotifyToRemoveVehicleFromTile(VehList[i].currTile);
            //compute new tile
            uint16_t maxTiles = RM.GetNumTiles();
                //vehicle at end of journey; wrap around
            if (maxTiles == VehList[i].currTile)
            {
                VehList[i].currTile = 0;
            }
            else
            {
                //just advance one tile
                VehList[i].currTile++;
            }
            RM.NotifyNewVehicleOnTile(VehList[i].currTile);
            VehList[i].diffOfCurrTile = RM.GetTileDifficulty(VehList[i].currTile);
            //update timestamp!
            VehList[i].timestampTileEntered = currTick;
            cout << VehList[i].vehEasyName << " has moved to " << RM.GetTileEasyName(VehList[i].currTile) << " on tick " << currTick << endl;
            cout << "\t Expected difficulty: " << RM.GetTileDifficulty(VehList[i].currTile) << endl;
        } //end if
    }//end for
}//end func

void DispatchManager::MainTask(TimeTracker& TT, RoadManager& RM)
{
    CheckIfAnyVehicleNeedsToStart(TT, RM);
    MoveVehicles(TT, RM);
}
