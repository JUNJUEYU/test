#pragma once
#include "eventInterface.h"
#include <map>
using namespace std;

typedef Event *(*DevEventFunc_t)(SubDevice *dev ,uint16_t msgID, const vector<uint8_t> &msg);

class TS30Event : public EventMaker
{
public:
    static TS30Event &getInstance(){
        static TS30Event instance;
        return instance;
    };
    // Event *createEvent(SubDevice *dev,const string &netData);
    Event *createEvent(const string &topic, const string &netData){};
    Event *createEvent(SubDevice *dev,uint16_t type, uint16_t msgID, const vector<uint8_t> &devData);
    Event *createEvent(SubDevice *dev,const string &type, const string &netData){};

private:
    void init();
    TS30Event();
    ~TS30Event();

private:
    map<uint16_t, DevEventFunc_t> devEventMap_m;
};

/* ----- End of file ----- */