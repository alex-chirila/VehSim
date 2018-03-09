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
        void ParseIniData(DispatchManager&, RoadManager&, TimeTracker&);
        //uint16_t nVehicles;
        //uint16_t nSegments;

    private:
        uint16_t getSectionOffset(string);
        uint16_t getSectionLength(string);
        bool findInSection(string secName, string whatToFind, string& retVal);
        void populateGeneralData(TimeTracker&);
        void populateRoadManagerData(RoadManager&);
        void populateVehicleData(DispatchManager&);
        void populateDriverData(DispatchManager&);
        void populateRouteData(DispatchManager&);

        string fullPathToIniFile;
        ifstream iniFPo; //file pointer
        string tempStr;

        typedef struct
        {
            string sectionName;
            uint16_t sectionOffset;
        } Section;

        typedef struct
        {
            string routeName;
            vector<uint16_t> segList; //first element is the start point
            vector<uint16_t> insertionPoints;
        } Route;

        vector<Section> SectList;
        vector<string> IniFileContents;
        vector<Route> RouteList;
};
