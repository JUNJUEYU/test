#include "eventInterface.h"
#include "eventManager.h"
#include "tools.h"

Event::Event(const string &uuid, const string &data) : 
    netData_m(data), queue_m(new BlockQueue(5)), dev_m(NULL)
{
    uuid_m = uuid;
}

Event::Event(SubDevice *dev ,uint16_t msgID ,const vector<uint8_t> &data) : 
        dev_m(dev),msgID_m(msgID), devData_m(data), queue_m(new BlockQueue(5))
{
    uuid_m = Uuid().creatUuid();
}

Event::~Event()
{
    delete queue_m;
    post();
}

string Event::getNetData(){
    return netData_m;
};
vector<uint8_t> Event::getDevData(){
    return devData_m;
};
uint16_t Event::getMsgID(){
    return msgID_m;
};
string Event::getUuid(){
    return uuid_m;
};

void Event::pend(uint32_t timeout){
    EventManager::getInstance().pendEvent(this, timeout);
}

void Event::post(){
    EventManager::getInstance().delPendEvent(this);
}

int Event::waitResponse(string &msg, uint32_t timeout){
    EventManager::getInstance().pendEvent(this, timeout);
    int len = queue_m->waitMsg(msg, timeout);

    return len;
}

void Event::pushMsg(const string &msg){
    queue_m->pushMsg(msg);
}
/* ----- End of file ----- */

