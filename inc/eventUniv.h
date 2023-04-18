#pragma once
#include "eventInterface.h"
#include <map>

typedef Event *(*UinvEventFunc_t)(SubDevice *dev ,uint16_t msgID,const vector<uint8_t> &msg);
typedef Event *(*NetEventFunc_t)(const string &topic ,const string &msg);

class UnivEvent : public EventMaker
{
public:
    static UnivEvent &getInstance(){
        static UnivEvent instance;
        return instance;
    };
    Event *createEvent(const string &topic, const string &netData);
    Event *createEvent(SubDevice *dev, uint16_t type, uint16_t msgID,const vector<uint8_t> &devData);
    Event *createEvent(SubDevice *dev,const string &type, const string &netData){};

private:
    UnivEvent();
    ~UnivEvent();

private:
    map<uint16_t, UinvEventFunc_t> univEventMap_m;
    map<string, NetEventFunc_t> netEventMap_m;
};

/* ----- End of file ----- */

