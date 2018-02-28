#include "Tools.h"
#include <sstream>

using namespace std;

string ConvertIntToString(uint16_t tempInt)
{
    stringstream s_s;
    s_s << tempInt;
    return s_s.str();
}
