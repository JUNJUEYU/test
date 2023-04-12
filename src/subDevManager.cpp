#include "subDevManager.h"
#include "tools.h"
#include "log.h"


void SubDevice::setParam(uint16_t zid, uint32_t nid){
    zid_m = zid;
    nid_m = nid;
    lastHeartTimeStamp_m = UtcTime::getUtcTimeStamp();
    survivalTime_m = 0;
};

Topology::Topology()
{
    devSnList_m.push_back("123456789");
}

Topology::~Topology()
{}

void SubDeviceManager::lock()
{
    pthread_mutex_lock(&mutex_m);
}

void SubDeviceManager::unlock()
{
    pthread_mutex_unlock(&mutex_m);
}


SubDeviceManager& SubDeviceManager::getInstance()
{
    static SubDeviceManager instance;
    return instance;
}

bool SubDeviceManager::creatSubDev(const string &devSn, uint16_t protVer)
{
    Lock lock(mutex_m);
    if(!topology_m->isPermised(devSn)){
        return false;
    }

    ProtAPI *protStack = pluginManager_m->getProt(protVer);
    if(NULL == protStack){
        return false;
    }

    SubDevice *dev = new SubDevice(devSn, protStack);
    dev->setParam(getFreeZid(devSn), getFreeNid(devSn));
    subDeviceList_m.push_back(dev);
    return true;
}

bool SubDeviceManager::creatSubDev(const string &devSn, uint16_t protVer, uint16_t zid)
{
    Lock lock(mutex_m);
    if(!topology_m->isPermised(devSn)){
        return false;
    }

    ProtAPI *protStack = pluginManager_m->getProt(protVer);
    if(NULL == protStack){
        return false;
    }

    SubDevice *dev = new SubDevice(devSn, protStack);
    dev->setParam(zid, getFreeNid(devSn));
    subDeviceList_m.push_back(dev);
    return true;
}

uint16_t SubDeviceManager::getFreeZid(const string &)
{
    return idPool_m->randomZid();
}


uint32_t SubDeviceManager::getFreeNid(const string &)
{
    return idPool_m->randomNid();
}


SubDeviceManager::SubDeviceManager()
{
    pluginManager_m = new PluginManager();
    topology_m = new Topology();
    idPool_m = new IdPool();
}

uint16_t SubDeviceManager::getVer(uint32_t &devNid)
{
    Lock lock(mutex_m);
    for(vector<SubDevice *>::iterator it = subDeviceList_m.begin(); it != subDeviceList_m.end(); ++it){
        if(devNid == (*it)->getNid()){
            return (*it)->getVer();
        }
    }

    return 0;
}

bool SubDeviceManager::verify(uint32_t nid)
{
    Lock lock(mutex_m);
    if(0 == nid){
        return true;
    }

    for(vector<SubDevice *>::iterator it = subDeviceList_m.begin(); it != subDeviceList_m.end(); ++it){
        if(nid == (*it)->getNid()){
            return true;
        }
    }

    return false;
}

uint16_t SubDeviceManager::getZid(const string &devSn)
{
    Lock lock(mutex_m);
    for(vector<SubDevice *>::iterator it = subDeviceList_m.begin(); it != subDeviceList_m.end(); ++it){
        if(devSn == (*it)->getSn()){
            return (*it)->getZid();
        }
    }

    return 0;
}

uint32_t SubDeviceManager::getNid(const string &devSn)
{
    Lock lock(mutex_m);
    for(vector<SubDevice *>::iterator it = subDeviceList_m.begin(); it != subDeviceList_m.end(); ++it){
        if(devSn == (*it)->getSn()){
            return (*it)->getNid();
        }
    }

    return 0;
}


/* ----- End of file ----- */
