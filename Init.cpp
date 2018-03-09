#include "Init.h"
#include "RM.h"
//#include <time.h>
#include <stdlib.h>
#include <chrono>
#include <string>
#include "Tools.h"
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
    //iniFPo = nullptr;
    PopulateSectionList();
}

void Initializer::ParseIniData(DispatchManager& DM, RoadManager& RM, TimeTracker& TT)
{
        //variables
    //uint16_t tempInt;
    //string tempStr1, tempStr2, tempStr3;
    //uint16_t i; //i-terator
    //string currSegmName, currSegmLength;
    //string currVeFU, currVeEN; //Fuel, EasyName
        //todo: push variables into corresponding for loop
        //for improved visibility
    //string currDrvName, currDrvStart, currDrvStop;

        //read in general data
    populateGeneralData(TT);

        //read in RM data
    populateRoadManagerData(RM);

    //read in Veh data
    populateVehicleData(DM);

    //read in driver data
    populateDriverData(DM);

    //read in route data
    populateRouteData(DM);

    //at this point, the slurped contents of the ini file are useless, can be discarded
    IniFileContents.clear();
        //create anonymous vector of same type as IniFileContents, but empty
        //"swap" elements between anonymous vector and IFC, thereby forcing IFC to deallocate all space
    vector<string>().swap(IniFileContents);
}

void Initializer::populateGeneralData(TimeTracker& TT)
{
    string tempStr = "";
    string currSectionName = "GENERAL";
    findInSection(currSectionName, "StartTime", tempStr);

    uint64_t tempLong = std::stoull(tempStr);

        #ifdef INIDEBUG
        cout << "Setting start time to : " << tempLong << endl;
        #endif // debug

    TT.setStartTick(tempLong);
    findInSection(currSectionName, "EndTime", tempStr);
    tempLong = std::stoull(tempStr);
    TT.setEndTick(tempLong);

        #ifdef INIDEBUG
        cout << "Setting end time to : " << tempLong << endl;
        #endif // debug
}

void Initializer::populateRoadManagerData(RoadManager& RM)
{
    string currSectionName = "ROAD";
    findInSection(currSectionName, "NumSegments", tempStr); ///TODO: remove tempStr from global class scope
    uint16_t nSegments = std::atoi(tempStr.c_str());
        #ifdef INIDEBUG
        cout << "N road segments in input file: " << nSegments << endl;
        #endif // debug

    uint16_t i; //iterator

    for (i=0; i < nSegments; i++)
    {
        string tempStr1 = "";

        tempStr1 = "Section" + ConvertIntToString(i) + "Name";
        string currSegmName;
        findInSection(currSectionName, tempStr1, currSegmName);

        tempStr1 = "Section" + ConvertIntToString(i) + "Length";
        string currSegmLength;
        findInSection(currSectionName, tempStr1, currSegmLength);

            #ifdef INIDEBUG
            cout << "\t\tRead road section [" << currSegmName << "] with difficulty [" << currSegmLength << "] from ini file." << endl;
            #endif // DEBUG
        RM.AddSegment(stoi(currSegmLength), currSegmName);
    }
}

void Initializer::populateVehicleData(DispatchManager& DM)
{
    string currSectionName = "VEHICLES";
    uint16_t nVehicles;
    findInSection(currSectionName, "NumVehicles", tempStr);
    nVehicles = std::atoi(tempStr.c_str());

        #ifdef INIDEBUG
        cout << "N Vehicles: " << nVehicles << endl;
        #endif // DEBUG

    uint16_t i; //iterator

    for (i=0; i < nVehicles; i++)
    {
        //build names
        tempStr2 = "Veh" + ConvertIntToString(i) + "Fuel";
        tempStr3 = "Veh" + ConvertIntToString(i) + "EasyName";
        findInSection(currSectionName, tempStr2, currVeFU);
        findInSection(currSectionName, tempStr3, currVeEN);
            #ifdef INIDEBUG
            cout << "\tSearched for labels: " << tempStr2 + ", " + tempStr3 << endl;
            cout << "\t\tRead vehicle: FUEL: " << currVeFU << " ; EN: " << currVeEN << endl;
            #endif // DEBUG
        DM.AddVehicle(stoi(currVeFU), currVeEN); //start time & tile to be updated later!
    }//end for

}

void Initializer::populateDriverData(DispatchManager& DM)
{
    currSectionName = "DRIVERS";
    findInSection(currSectionName, "NDrivers", tempStr);
    tempInt = std::atoi(tempStr.c_str());
        #ifdef INIDEBUG
        cout << "N Drivers: " << tempInt << endl;
        #endif // INIDEBUG
    for (i=0; i < tempInt; i++)
    {
        string tempStr1, currDrvName, currDrvStart, currDrvStop;

        //build var names
        tempStr1 = "D" + ConvertIntToString(i) + "N";
        findInSection(currSectionName, tempStr1, currDrvName);
        tempStr1 = "D" + ConvertIntToString(i) + "STT";
        findInSection(currSectionName, tempStr1, currDrvStart);
        tempStr1 = "D" + ConvertIntToString(i) + "STP";
        findInSection(currSectionName, tempStr1, currDrvStop);
        DM.AddDriver(currDrvName, (uint32_t)stoi(currDrvStart), (uint32_t)stoi(currDrvStop));

    }//end for
}

void Initializer::populateRouteData(DispatchManager& DM)
{
    //read in route data
    currSectionName = "ROUTES";
    findInSection(currSectionName, "NRoutes", tempStr);
    tempInt = std::atoi(tempStr.c_str());

    for (i=0; i < tempInt; i++)
    {
        string thisIname = ConvertIntToString(i);

            //read route ID
        tempStr1 = "Route" + thisIname + "ID";
        findInSection(currSectionName, tempStr1, thisIname);

    }
}

bool Initializer::findInSection(string secName, string whatToFind, string& retVal)
{
    uint16_t i;
    bool searchSuccess = false;
    uint16_t startLine = getSectionOffset(secName);
    uint16_t nrLines = getSectionLength(secName);
    uint16_t positionOfEqual;

    for (i = startLine; i <= (startLine+nrLines); i++)
    {
        if (std::string::npos != IniFileContents[i].find(whatToFind))
        {
            //One line contains the required text. Find the position of the "=" sign.
            positionOfEqual = IniFileContents[i].find("=");
            //return the substring after the "=" sign (+ a space)
            retVal = IniFileContents[i].substr(positionOfEqual+2, std::string::npos);
            searchSuccess = true;
        }
    }

    return searchSuccess;
}

void Initializer::PopulateSectionList()
{
    string sectionString = "[SECTION]";
    iniFPo.open(fullPathToIniFile.c_str());
    if (!iniFPo)
    {
        cout << "Unable to read ini file :( " << endl;
    }
    IniFileContents.push_back("0000000000000000"); //push dummy line to make array start at 1
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
            tempSection.sectionOffset = i+2; //contents start AFTER line with section name
            SectList.push_back(tempSection);
                //DEBUG
            //cout << "Found section " << tempSection.sectionName << " on line " << tempSection.sectionOffset << endl;
        }
    }
    //insert one last dummy section, for easier computation
    tempSection.sectionName = "DUMMY";
    tempSection.sectionOffset = i; //iterator was incremented on last step of loop
    SectList.push_back(tempSection);
}

///Returns MAXUINT16_T (65535) if section does not exist. Otherwise returns section number.
uint16_t Initializer::getSectionOffset(string sectName)
{
    uint16_t i, retVal;
    retVal = 65535;

    for (i=0; i < SectList.size(); i++)
    {
        if (SectList[i].sectionName == sectName)
        {
            retVal = SectList[i].sectionOffset;
        }
    }//end for
        #ifdef INIDEBUG
        if (retVal == 65535)
        {
            cout << "\t Section [" << sectName << "] could not be found in order to determine startpoint" << endl;
        }
        #endif // DEBUG
    return retVal;
}

///Returns MAX UINT16_T if section does not exist. Otherwise returns section length.
uint16_t Initializer::getSectionLength(string sectName)
{
    uint16_t i;
    uint16_t retVal = 65535;
    for (i=0; i < SectList.size(); i++)
    {
        if (SectList[i].sectionName == sectName)
        {
            retVal = SectList[i+1].sectionOffset - SectList[i].sectionOffset - 2; //offsetting includes two lines with Section + Name
        }
    }//end for
        #ifdef INIDEBUG
        if (retVal == 65535)
        {
            cout << "\t Section [" << sectName << "] could not be found in order to determine length" << endl;
        }
        #endif // DEBUG
    return retVal;
}
