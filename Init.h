#include <iostream>
#include <string>
#include "DM.h"
#include "RM.h"
#include <fstream>

class Initializer
{
    public:
        Initializer();
        //void readRoadParams(RoadManager&); //road needs to be initialized first, otherwise vehicles cannot occupy tiles
        //void readVehData(DispatchManager&);
        void PopulateSectionList();
        void ParseIniData(DispatchManager&, RoadManager&);
        //uint16_t nVehicles;
        //uint16_t nSegments;

    private:
        //void readSegmentParams(uint16_t);
            //obsolete
        //uint16_t thisVStartTile, thisVStartTick;
        //uint16_t thisSegDifficulty;
            //obsolete: variables moved to own function
        string fullPathToIniFile;
        //string tileDataFName;
        //string vehDataFName;
        //ifstream tileDataFPtr;
        //ifstream vehDataFP;
        ifstream iniFPo; //file pointer
        string tempStr;

        typedef struct
        {
            string sectionName;
            uint16_t sectionOffset;
        } Section;

        vector<Section> SectList;
        vector<string> IniFileContents;
};
