#include <iostream>
#include <cstdint>
#include "DM.h"

using namespace std;

DispatchManager::DispatchManager()
{
    //do nothing
    currVehNo = 0;
    nIdleVehs = 0;
}

void DispatchManager::AddVehicle(uint16_t startTile, uint16_t startTick, string vehName)
{
    nIdleVehs++;
    Vehicle tempVeh;
    tempVeh.sTick = startTick;
    tempVeh.currTile = startTile;
    tempVeh.sTile = startTile;
    tempVeh.state = STARTIDLE;
    tempVeh.vehID = currVehNo;
    tempVeh.vehEasyName = vehName;
    //todo: add error-handling in case the vehicle cant be added to list
    VehList.push_back(tempVeh);

    currVehNo++;
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

void DispatchManager::checkIfAnyVehicleNeedsToStart(TimeTracker& TT, RoadManager& RM)
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
                RM.notifyNewVehicleOnTile(VehList[i].sTile);
                VehList[i].diffOfCurrTile = RM.getTileDifficulty(VehList[i].sTile);
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
void DispatchManager::moveVehicles(TimeTracker& TT, RoadManager& RM)
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
            RM.notifyToRemoveVehicleFromTile(VehList[i].currTile);
            //compute new tile
            uint16_t maxTiles = RM.getNumTiles();
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
            RM.notifyNewVehicleOnTile(VehList[i].currTile);
            VehList[i].diffOfCurrTile = RM.getTileDifficulty(VehList[i].currTile);
            //update timestamp!
            VehList[i].timestampTileEntered = currTick;
            cout << VehList[i].vehEasyName << " has moved to " << RM.getTileEasyName(VehList[i].currTile) << " on tick " << currTick << endl;
            cout << "\t Expected difficulty: " << RM.getTileDifficulty(VehList[i].currTile) << endl;
        } //end if
    }//end for
}//end func

void DispatchManager::MainTask(TimeTracker& TT, RoadManager& RM)
{
    checkIfAnyVehicleNeedsToStart(TT, RM);
    moveVehicles(TT, RM);
}
