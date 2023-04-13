#pragma once
#include "eventInterface.h"
#include <map>
using namespace std;

typedef Event *(*NetEventFunc_t)(SubDevice *dev ,const string &);
typedef Event *(*DevEventFunc_t)(SubDevice *dev ,const vector<uint8_t> &);

class TS30Event : public EventMaker
{
public:
    static TS30Event &getInstance(){
        static TS30Event instance;
        return instance;
    };
    Event *createEvent(SubDevice *dev,const string &netData);
    Event *createEvent(SubDevice *dev,const vector<uint8_t> &devData);

private:
    void init();
    TS30Event();
    ~TS30Event();

private:
    map<string, NetEventFunc_t> netEventMap_m;
    map<uint16_t, DevEventFunc_t> devEventMap_m;
};

/* ----- End of file ----- */