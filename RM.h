#pragma once

#include <cstdint>
#include <vector>
#include "OS.h"
#include <string>

using namespace std;

class RoadManager
{
    public:

        RoadManager();

        void AddSegment(uint16_t difficulty, string segEasyName);
        void MainTask(TimeTracker&);
        void NotifyNewVehicleOnTile(uint16_t tileNo);
        void NotifyToRemoveVehicleFromTile(uint16_t tileNo);
        void ListTiles();
        uint16_t GetTileDifficulty(uint16_t tileNo);
        uint16_t GetNumTiles();
        string GetTileEasyName(uint16_t id);

    private:
        //void checkIdleVehList();

        enum tileType
        {
            STATION,
            REGULAR,
            TLIGHT
        };

        typedef struct
        {
            uint16_t nVehiclesOnTile;
            uint16_t baseDifficulty;
            uint16_t currDifficulty;
            uint16_t chanceToChange; //chance for a random change in difficulty
            uint16_t intervalToChange; //interval for random changes
            uint16_t timeSinceLastChange; // time since last random change
            uint16_t changeBoundLow; //in percentage points
            uint16_t changeBoundHigh; // in percentage points
            bool vehPresent;
            string segEasyName;
            tileType segType;
        } Tile;

        uint16_t nextSegNo; //variable to point to the top of the stack (+1)
        uint16_t workingSegNo; //variable to indicate on which segment is the current operation happening
        vector<Tile> SegList;
};
