#include "RM.h"
#include <iostream>
#include <string>

using namespace std;

inline const char* const BoolToString(bool b)
{
    return b ? "true" : "false";
}

RoadManager::RoadManager()
{
    nextSegNo = 0;
    workingSegNo = -1;
    //do nothing
}

void RoadManager::AddSegment(uint16_t setDifficulty, string segEasyName)
{
    Tile tempTile;

    tempTile.baseDifficulty = setDifficulty;
    tempTile.currDifficulty = setDifficulty;
    tempTile.segEasyName = segEasyName;
    tempTile.vehPresent = false;
        ///Todo: read this data from the initializer, too
    tempTile.chanceToChange = 20;
    tempTile.changeBoundHigh = 100;
    tempTile.changeBoundLow = 10;
    tempTile.intervalToChange = 100;
    tempTile.timeSinceLastChange = 0;
        //
    tempTile.nVehiclesOnTile = 0;
    tempTile.segType = REGULAR; //default type if prefix does not indicate otherwise
    if (segEasyName.find("SEM-"))
    {
        tempTile.segType = TLIGHT;
    }
    if (segEasyName.find("ST-"))
    {
        tempTile.segType = STATION;
    }
    SegList.push_back(tempTile);
        #ifdef DEBUG
        cout << "Adding road segment: " << endl;
        cout << "\t[" << tempTile.segEasyName << "], difficulty [" << tempTile.baseDifficulty << "]" << endl;
        #endif // DEBUG
    nextSegNo++;
}

string RoadManager::GetTileEasyName(uint16_t id)
{
    return SegList[id].segEasyName;
}

///DEBUG FUNC
void RoadManager::ListTiles()
{
    cout << "LIST OF SEGMENTS:" << endl;
    cout << "-----------------" << endl << endl;
    cout << "Total no. of segments: " << nextSegNo << endl;
    for (int i = 0; i < nextSegNo; i++)
    {
        cout << "Segment #" << i << ":" << endl;
        cout << "\t base difficulty: " << SegList[i].baseDifficulty << endl;
        cout << "\t curr difficulty: " << SegList[i].currDifficulty << endl;
        cout << "\t has X vehicles: " << SegList[i].nVehiclesOnTile << endl;
        //cout << "\t chance to change difficulty: " << SegList[i].chanceToChange << endl;
    }
    cout << endl;
}

void RoadManager::NotifyNewVehicleOnTile(uint16_t tileNo)
{
    if (tileNo <= (nextSegNo-1))
    {
        //cout << "Adding a vehicle to tile: " << tileNo << endl;
        SegList[tileNo].vehPresent = true;
        SegList[tileNo].nVehiclesOnTile++;
    }
    else
    {
        cout << "Request denied. Cannot place vehicle on tile " << tileNo;
        cout << ", because I have only " << nextSegNo-1 << " tiles available!" << endl;
    }
}

void RoadManager::NotifyToRemoveVehicleFromTile(uint16_t tileNo)
{
    if (tileNo <= (nextSegNo-1))
    {
        //cout << "A vehicle has left segment " << tileNo << endl;
        if (SegList[tileNo].nVehiclesOnTile > 0)
        {
            SegList[tileNo].nVehiclesOnTile--;
            if (0 == SegList[tileNo].nVehiclesOnTile)
            {
                SegList[tileNo].vehPresent = false;
            }
            //else : At least one vehicle is still present!
        }
        else
        {
            cout << "Can't remove vehicle from tile " << tileNo << endl;
            cout << "Number of vehicles is 0!" << endl;
        }
    }
    else
    {
        cout << "Request denied. Cannot remove vehicle from tile " << tileNo;
        cout << ", because only " << nextSegNo-1 << " tiles exist!" << endl;
    }
}

uint16_t RoadManager::GetTileDifficulty(uint16_t tileNo)
{
    return SegList[tileNo].currDifficulty;
}

uint16_t RoadManager::GetNumTiles()
{
    return (nextSegNo - 1);
}

void RoadManager::MainTask(TimeTracker& TT)
{
    ///TODO: Add code for the random changing of tile difficulty.
}
