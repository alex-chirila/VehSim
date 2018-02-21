#include "Init.h"
#include "RM.h"
//#include <time.h>
#include <stdlib.h>
#include <chrono>
//#include <thread>

using namespace std;

Initializer::Initializer()
{
    //nVehicles = 0;
    //nSegments = 0;
    fullPathToIniFile = "c:\\Users\\alexandru.chirila\\Desktop\\CB\\VehSim\\bin\\Release\\ini.txt";
    //tileDataFName = "tiles.txt";
    //vehDataFName = "veh.txt";
    tempStr = "";
    iniFPo = nullptr;
    PopulateSectionList();
}

void Initializer::ParseIniData(DispatchManager& DM, RoadManager& RM)
{
    //variables

    //read in general data

    //read in RM data

    //read in DM data
}

void Initializer::PopulateSectionList()
{
    string sectionString = "[SECTION]";
    iniFPo.open(fullPathToIniFile.c_str());
    if (!iniFPo)
    {
        cout << "Unable to read ini file :( " << endl;
    }
    if (iniFPo.is_open())
    {
        while (getline(iniFPo, tempStr))
            //should auto-quit when reaching EOF
        {
            //slurp all lines into a vector
            IniFileContents.push_back(tempStr);
        }
    }
    iniFPo.close();

    Section tempSection; //placeholder for Sections inserted into vector
    uint16_t i;
    for (i = 0; i < IniFileContents.size(); i++)
    {
        string tempStr = IniFileContents[i];

        if (std::string::npos != tempStr.find(sectionString))
        {
            //this is a section string!
            tempSection.sectionName = IniFileContents[i+1]; //potential out-of-bounds bug
            tempSection.sectionOffset = i;
            SectList.push_back(tempSection);
                //DEBUG
            cout << "Found section " << tempSection.sectionName << " on line " << i << endl;
        }
    }
    //insert one last dummy section, for easier computation
    tempSection.sectionName = "DUMMY";
    tempSection.sectionOffset = i; //iterator was incremented on last step of loop
}

/*
void Initializer::readVehData(DispatchManager& DM)
{
    //todo: add a function to read from text file

    string vehDataFilePath = fullIniPath + vehDataFName;
    vehDataFP.open(vehDataFilePath.c_str());
    if (!vehDataFP)
    {
        cout << "Unable to read file containing vehicle data :( " << endl;
    }

    //if successfully open, read data!
    if (vehDataFP.is_open())
    {
        //read line 1: number of vehicles
        getline(vehDataFP, tempStr);
        nVehicles = atoi(tempStr.c_str());
        cout << "nVehicles " << nVehicles << endl;

        //read each veh data in a loop
        for (int i=0; i < nVehicles; i++)
        {
            //read start tick
            getline(vehDataFP, tempStr);
            thisVStartTick = atoi(tempStr.c_str());
            //read start tile
            getline(vehDataFP, tempStr);
            thisVStartTile = atoi(tempStr.c_str());
            //read name
            getline(vehDataFP, tempStr);
            //push new vehicle into stack
            DM.AddVehicle(thisVStartTile, thisVStartTick, tempStr);
            //do NOT notify RM about vehicle on tile
            //even if it's supposed to start at tick 0, DispatchManager should handle this!
        }
    }
    else
    {
        cout << "Unable to open file with tile info." << endl;
    }
}

void Initializer::readRoadParams(RoadManager& RM)
{
    string roadDataFName = fullIniPath + tileDataFName;
    tileDataFPtr.open(roadDataFName.c_str());

    if (tileDataFPtr.is_open())
    {
        //step 1: get number of tiles
        getline(tileDataFPtr, tempStr);
        nSegments = atoi(tempStr.c_str());
        cout << "Ini: read segments: " << nSegments << endl;

        for (int i=0; i < nSegments; i++)
        {
            //read this tile's difficulty
            getline(tileDataFPtr, tempStr);
            thisSegDifficulty = atoi(tempStr.c_str());
            //read this tile's easy name
            getline(tileDataFPtr, tempStr);
            RM.AddSegment(thisSegDifficulty, tempStr);
        }

    }
}

void Initializer::readSegmentParams(uint16_t segNo)
{
    //todo: add real code
    thisSegDifficulty = 10 + (rand() % 5 + 1);
}
*/
