#include "tools.h"
#include "log.h"
#include <time.h>

uint32_t UtcTime::getUtcTimeStamp()
{
    time_t t = time(NULL);
    return t;
}


string Bcd::toDecString()
{
    string decStr;
    for(vector<uint8_t>::iterator it = bcdData_m.begin(); it != bcdData_m.end(); it++){
        uint8_t high = (*it) >> 4;
        uint8_t low = (*it) & 0x0f;
        decStr += high + '0';
        decStr += low + '0';
    }
    return decStr;
}

vector<uint8_t> Bcd::toBcd()
{
    vector<uint8_t> bcd;
    for(string::iterator it = decData_m.begin(); it != decData_m.end(); it++){
        uint8_t high = (*it) >> 4;
        uint8_t low = (*it) & 0x0f;
        bcd.push_back(high);
        bcd.push_back(low);
    }
    return bcd;
}


uint8_t Sum::check(){
    uint8_t sum = 0;
    for(vector<uint8_t>::iterator it = data_m.begin(); it != data_m.end(); it++){
        sum += *it;
    }
    return sum;
}

bool Sum::operator==(uint8_t sum)
{
    return sum_m == sum;
}

bool Sum::operator!=(uint8_t sum)
{
    return sum_m != sum;
}
