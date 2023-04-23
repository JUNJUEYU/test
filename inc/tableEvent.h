#pragma once
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <stdint.h>
#include "paramBase.h"
using namespace std;

class Event
{
    public:
        string name_m;
        string object_m;
        string type_m;
        uint32_t regAddr_m;
        vector<ParamBase> param_m;
};

class EventTable
{
    public:
        static EventTable& getInstance();
        void addEvent(Event evt);
        bool getEvent(string name, Event &evt);
        bool getEvent(uint32_t regAddr, Event &evt);
        int getEventSize();
    private:
        EventTable();
        EventTable(const EventTable&);
        EventTable& operator=(const EventTable&);
        vector<Event> evtList_m;
};

