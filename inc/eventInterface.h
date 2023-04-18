#pragma once
#include <stdint.h>
#include <string>
#include "msgQueue.h"
#include "subDevManager.h"
#include "myThread.h"
using namespace std;

class Event
{
public:
    Event(const string &uuid, const string &data);
    Event(SubDevice *dev ,uint16_t msgID ,const vector<uint8_t> &data);
    virtual ~Event();
    virtual uint8_t run() = 0;
    string getNetData();
    vector<uint8_t> getDevData();
    uint16_t getMsgID();
    string getUuid();
    int waitResponse(string &msg, uint32_t timeout = 0);
    void pushMsg(const string &msg);

private:
    void post();
    void pend(uint32_t timeout = 0);

protected:
    SubDevice *dev_m;
    string netData_m;
    vector<uint8_t> devData_m;
    uint16_t msgID_m;
    string uuid_m;
    BlockQueue *queue_m;
};

class EventMaker
{
public:
    EventMaker(){
    };
    EventMaker(SubDevice *dev,const vector<uint8_t> &data){};

    virtual ~EventMaker(){};
    virtual Event *createEvent(const string &topic, const string &netData) = 0;
    virtual Event *createEvent(SubDevice *dev,const string &type ,const string &netData) = 0;
    virtual Event *createEvent(SubDevice *dev, uint16_t type,uint16_t msgID, const vector<uint8_t> &devData) = 0;
};

/* ----- End of file ----- */
