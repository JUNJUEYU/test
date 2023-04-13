#pragma once
#include <stdint.h>
#include <string>
#include "msgQueue.h"
#include "subDevManager.h"
#include "myThread.h"
using namespace std;

class EventParam
{
public:
    EventParam(){
        netMsgQueue_m = &NetMsgSendQueue::getInstance();
        devMsgQueue_m = &DevMsgSendQueue::getInstance();
        subDevManager_m = &SubDeviceManager::getInstance();
        threadManager_m = &ThreadManager::getInstance();
    };
    virtual ~EventParam(){};

public:
    MessageQueue<string> *netMsgQueue_m;
    MessageQueue<vector<uint8_t> > *devMsgQueue_m;
    SubDeviceManager *subDevManager_m;
    ThreadManager *threadManager_m;
};

class Event
{
public:
    Event(SubDevice *dev ,const string &data) : eventParam_m(new EventParam),dev_m(dev), netData_m(data){};
    Event(SubDevice *dev ,const vector<uint8_t> &data) : eventParam_m(new EventParam),dev_m(dev), devData_m(data){};
    virtual ~Event(){
        delete eventParam_m;
    };
    virtual uint8_t run() = 0;
    string getNetData(){
        return netData_m;
    };
    vector<uint8_t> getDevData(){
        return devData_m;
    };
protected:
    EventParam *eventParam_m;
    SubDevice *dev_m;
    string netData_m;
    vector<uint8_t> devData_m;
};

class EventMaker
{
public:
    EventMaker(){
    };
    EventMaker(SubDevice *dev,const vector<uint8_t> &data){};

    virtual ~EventMaker(){};
    virtual Event *createEvent(SubDevice *dev, const string &netData) = 0;
    virtual Event *createEvent(SubDevice *dev, const vector<uint8_t> &devData) = 0;
};

/* ----- End of file ----- */
