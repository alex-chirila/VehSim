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

        enum routeInfoType
        {
            ID,
            STT, //start time of route
            STP, //end time of route
            SEG, //another segment number
            IP, //insertion point number
            TIS, //time-interval start-time
            TIVC, //time-interval vehicle count
        };

            //adders
        void AddVehicle(uint16_t fuelLevel, string vehName);
        void AddDriver(string name, uint32_t recommendedStart, uint32_t recommendedStop);
        void AddInfoToRoute(routeInfoType, string);

        void MainTask(TimeTracker&, RoadManager&);

            //helpers
        void ListVehicles();

            //actual work
        void CheckIfAnyVehicleNeedsToStart(TimeTracker&, RoadManager&);
        void MoveVehicles(TimeTracker&, RoadManager&);

    private:
        uint16_t currVehNo;
        uint16_t nIdleVehs;
        uint16_t nDrivers;

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
                //actually driving
            DRIVING,
                //driving but told by DM to hold position
            IDLE,
                //not assigned to vehicle, default
            NOTASSIGNED
        };

        typedef struct
        {
            string driverName;
            uint32_t availableStartTime;
            uint32_t availableStopTime;
            uint32_t todayStartTime;
            uint32_t todayStopTime;
            uint16_t assignedVehicle; //array index in vehicle vector
            driverStatus status;
        } Driver;

        vector<Vehicle> VehList;
        vector<Driver> DriverList;
};
