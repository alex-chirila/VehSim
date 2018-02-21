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
        void notifyNewVehicleOnTile(uint16_t tileNo);
        void notifyToRemoveVehicleFromTile(uint16_t tileNo);
        void ListTiles();
        uint16_t getTileDifficulty(uint16_t tileNo);
        uint16_t getNumTiles();
        string getTileEasyName(uint16_t id);

    private:
        void Veh_CheckIdleList();
        void HandleTrafficLights();

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

        typedef struct
        {
            uint64_t startTime;
            uint64_t endTime;
            uint16_t lineToChange; //vehicle line that will change
            uint16_t chanceNewTraveler; //chance a new traveler will appear every tick
            uint16_t chanceLeaveTraveler; //chance an existing traveler will leave station (boredom etc)
            uint16_t difficultyPer5Travelers; //how many extra ticks are needed per 5 travelers
        } TimeInterval;

        uint16_t nextSegNo; //variable to point to the top of the stack (+1)
        uint16_t workingSegNo; //variable to indicate on which segment is the current operation happening
        vector<Tile> SegList;
};
