#pragma once
#include <cstdint>
#include <vector>
#include "OS.h"
#include "RM.h"

using namespace std;

class DispatchManager
{
    public:
        DispatchManager();
        void AddVehicle(uint16_t fuelLevel, string vehName);
        void MainTask(TimeTracker&, RoadManager&);
        void ListVehicles();
        void checkIfAnyVehicleNeedsToStart(TimeTracker&, RoadManager&);
        void moveVehicles(TimeTracker&, RoadManager&);

    private:
        uint16_t currVehNo;
        uint16_t nIdleVehs;

        enum vehState
        {
            STARTIDLE,
            MOVING,
            ORDEREDIDLE
        };

        typedef struct
        {
            uint16_t sTick;
            uint16_t fuelLevel;
            uint16_t sTile;
            uint16_t currTile;
            uint64_t timestampTileEntered;
            uint16_t vehID;
            uint64_t timestampIdle;
            uint16_t howLongToIdle;
            vehState state;
            string vehEasyName;
            uint16_t diffOfCurrTile;

        } Vehicle;

        enum driverStatus
        {
            DRIVING,
            IDLING
        };

        typedef struct
        {
            string driverID;
            uint32_t availableStartTime;
            uint32_t availableStopTime;
            driverStatus status;
        } Driver;

        vector<Vehicle> VehList;
};
