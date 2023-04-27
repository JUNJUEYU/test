#pragma once
#include <string>
#include <vector>
#include <stdint.h>
using namespace std;

class EventItem{
    public:
        string type;
        string  object;
        uint16_t reg;
};

class EventDesc{
    public:
        string name;
        vector<EventItem> items;
};

class DescTblEvent{
    public:
        DescTblEvent(const string &path);
        bool getEventDesc(uint16_t eventCode, EventDesc &eventDesc);
        int getEventDescSize();
        
    private:
        string pd_m;
        vector<EventDesc> eventList_m;
};