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
    uint64_t tempLong;
    uint16_t tempInt;
    string tempStr1, tempStr2, tempStr3;
    uint16_t i; //i-terator
    string currSecName; //section in INI FILE
    string currSegmName, currSegmLength;
    string currVeOF, currVeSG, currVeEN; //StartTick, SeGment, EasyName

        //read in general data
    currSecName = "GENERAL";
    findInSection(currSecName, "StartTime", tempStr);
    tempLong = std::stoull(tempStr);
        #ifdef INIDEBUG
        cout << "Setting start time to : " << tempLong << endl;
        #endif // debug
    TT.setStartTick(tempLong);
    findInSection(currSecName, "EndTime", tempStr);
    tempLong = std::stoull(tempStr);
    TT.setEndTick(tempLong);
        #ifdef INIDEBUG
        cout << "Setting end time to : " << tempLong << endl;
        #endif // debug

        //read in RM data
    currSecName = "ROAD";
    findInSection(currSecName, "NumSegments", tempStr);
    tempInt = std::atoi(tempStr.c_str());
        #ifdef INIDEBUG
        cout << "N road segments in input file: " << tempInt << endl;
        #endif // debug
    for (i=0; i < tempInt; i++)
    {
        tempStr1 = "Section" + ConvertIntToString(i) + "Name";
        tempStr2 = "Section" + ConvertIntToString(i) + "Length";
        findInSection(currSecName, tempStr1, currSegmName);
        findInSection(currSecName, tempStr2, currSegmLength);
            #ifdef INIDEBUG
            cout << "\tSearched for labels " << tempStr1 << " and " << tempStr2 << endl;
            cout << "\t\tRead road section [" << currSegmName << "] with difficulty [" << currSegmLength << "]" << endl;
            #endif // DEBUG
        RM.AddSegment(stoi(currSegmLength), currSegmName);
    }
    //read in DM data
    currSecName = "VEHICLES";
    findInSection(currSecName, "NumVehicles", tempStr);
    tempInt = std::atoi(tempStr.c_str());
        #ifdef INIDEBUG
        cout << "N Vehicles: " << tempInt << endl;
        #endif // DEBUG
    for (i=0; i < tempInt; i++)
    {
        //build names
        tempStr1 = "Veh" + ConvertIntToString(i) + "TickOffset";
        tempStr2 = "Veh" + ConvertIntToString(i) + "Segment";
        tempStr3 = "Veh" + ConvertIntToString(i) + "EasyName";
        findInSection(currSecName, tempStr1, currVeOF);
        findInSection(currSecName, tempStr2, currVeSG);
        findInSection(currSecName, tempStr3, currVeEN);
            #ifdef INIDEBUG
            cout << "\tSearched for labels: " << tempStr1 + ", " + tempStr2 + ", " + tempStr3 << endl;
            cout << "\tRead vehicle: OF " << currVeOF << " SG " << currVeSG << " EN: " << currVeEN << endl;
            #endif // DEBUG
        DM.AddVehicle(stoi(currVeSG), stoi(currVeOF)+TT.getStartTick(), currVeEN);
    }//end for
}

/*
bool Initializer::findInSection(string secName, string whatToFind, string& retVal)
{
    uint16_t i;
    bool searchSuccess = false;
    uint16_t startLine = getSectionOffset(secName);
    uint16_t nrLines = getSectionLength(secName);

    for (i = startLine; i <= (startLine+nrLines); i++)
    {
        if (std::string::npos != IniFileContents[i].find(whatToFind))
        {
            //one line contains the required text!
            retVal = IniFileContents[i+1];
            searchSuccess = true;
        }
    }

    return searchSuccess;
} */

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
