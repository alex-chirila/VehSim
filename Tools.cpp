#include "Tools.h"
#include <sstream>

using namespace std;

string ConvertIntToString(uint16_t tempInt)
{
    stringstream s_s;
    s_s << tempInt;
    return s_s.str();
}

string ConvertTimeStampToHHMM(uint64_t timeStamp)
{
        //Step 1 : Remove multiples of day and store the information
    uint16_t nDays = timeStamp / 86400;
    timeStamp = timeStamp % 86400;
        //Step 2 : Extract the "minutes" information
    uint16_t hours = timeStamp / 3600;
    uint16_t minutes = timeStamp % 3600;

    string retVal = "";
    retVal = ConvertIntToString(nDays) + "days";
    retVal = retVal + "; " + ConvertIntToString(hours) + "h:";
    retVal = retVal + ConvertIntToString(minutes) + "m";

    return retVal;
}
