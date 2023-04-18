#include "log.h"
#include <iostream>
#include <iomanip>

void Log::init()
{
    elog_init(LOG_FILE, LOG_SIZE, LOG_COUNT);
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL &~ ELOG_FMT_P_INFO &~ ELOG_FMT_TAG &~ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_ALL &~ ELOG_FMT_P_INFO &~ ELOG_FMT_TAG &~ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TIME);
    elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL &~ ELOG_FMT_P_INFO &~ ELOG_FMT_TAG &~ELOG_FMT_FUNC);
    elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL  &~ ELOG_FMT_P_INFO &~ ELOG_FMT_TAG &~ELOG_FMT_FUNC);
    elog_start();
}

void Log::print(const char *msg)
{
    cout<<msg<<endl;
}

void Log::print(const char *msg, int value)
{
    cout<<msg<<value<<endl;
}

void Log::print(const char *msg, const char *value)
{
    cout<<msg<<value<<endl;
}

void Log::print(const char *msg, vector<uint8_t> value)
{
    cout<<msg<<" = ";
    for (int i = 0; i < value.size(); i++)
    {
        cout<<hex<<setw(2)<<setfill('0')<<(int)value[i]<<" ";
    }
    cout<<endl;
}

void Log::print(const char *msg, const string & value)
{
    cout<<msg<<value<<endl;
}

void Log::error(const char *msg)
{
    cout<<"error: "<<msg<<endl;
}

void Log::error(const char *msg, int value)
{
    cout<<"error: "<<msg<<value<<endl;
}

void Log::error(const char *msg, const char *value)
{
    cout<<"error: "<<msg<<value<<endl;
}

void Log::error(const char *msg, vector<uint8_t> value)
{
    cout<<"error: "<<msg;
    for (int i = 0; i < value.size(); i++)
    {
        cout<<hex<<setw(2)<<setfill('0')<<(int)value[i]<<" ";
    }
    cout<<endl;
}

/* ----- End of file ----- */
