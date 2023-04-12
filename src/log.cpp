#include "log.h"
#include <iostream>
#include <iomanip>

void Log::print(const char *msg)
{
    cout<<msg<<endl;
}

void Log::print(const char *msg, int value)
{
    cout<<msg<<value<<endl;
}

void Log::print(const char *msg, vector<uint8_t> value)
{
    cout<<msg;
    for (int i = 0; i < value.size(); i++)
    {
        cout<<hex<<setw(2)<<setfill('0')<<(int)value[i]<<" ";
    }
    cout<<endl;
}


/* ----- End of file ----- */
