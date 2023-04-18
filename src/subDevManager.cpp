#include "subDevManager.h"
#include "tools.h"
#include "log.h"
#include "db.h"


void SubDevice::setParam(uint16_t zid, uint32_t nid){
    zid_m = zid;
    nid_m = nid;
    lastHeartTimeStamp_m = UtcTime::getUtcTimeStamp();
    survivalTime_m = 0;
};


SubDevice::SubDevice(const string &sn, uint16_t ver)
{
    sn_m = sn;
    zid_m = 0;
    nid_m = 0;
    survivalTime_m = 0;
    lastHeartTimeStamp_m = 0;
    ver_m = ver;
    name_m = "TS30";
}

SubDevice::SubDevice(const string &sn, uint16_t ver, uint16_t zid)
{
    sn_m = sn;
    zid_m = zid;
    nid_m = 0;
    survivalTime_m = 0;
    lastHeartTimeStamp_m = 0;
    ver_m = ver;
    name_m = "TS30";
}


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


string SubDeviceManager::verify(uint32_t srcNid, uint32_t tarNid)
{
    Lock lock(mutex_m);

    if(1 != tarNid){
        return "illegal";
    }

    if(0 == srcNid){
        return "netAccess";
    }

    Db db;
    if(!db.isRegisted(srcNid)){
        return "illegal";
    }

    return "normal";
}



SubDevice *SubDeviceManager::getSubDev(uint32_t nid)
{
    Lock lock(mutex_m);
    for(vector<SubDevice *>::iterator it = subDeviceList_m.begin(); it != subDeviceList_m.end(); ++it){
        if(nid == (*it)->getNid()){
            log_d("SubDeviceManager::getSubDev find *it = %p", *it);
            return *it;
        }
    }

    log_d("SubDeviceManager::getSubDev not find");
    return NULL;
}

SubDevice *SubDeviceManager::getSubDev(const string &sn)
{
    Lock lock(mutex_m);
    for(vector<SubDevice *>::iterator it = subDeviceList_m.begin(); it != subDeviceList_m.end(); ++it){
        if(sn == (*it)->getSn()){
            return *it;
        }
    }

    return NULL;
}

void SubDeviceManager::deleteTmpDev(const string &sn)
{
    Lock lock(mutex_m);
    for(vector<SubDevice *>::iterator it = tmpDevList_m.begin(); it != tmpDevList_m.end(); ++it){
        if(sn == (*it)->getSn()){
            tmpDevList_m.erase(it);
            delete *it;
            return;
        }
    }
}

SubDevice *SubDeviceManager::getTmpDev(const string &sn)
{
    Lock lock(mutex_m);
    for(vector<SubDevice *>::iterator it = tmpDevList_m.begin(); it != tmpDevList_m.end(); ++it){
        if(sn == (*it)->getSn()){
            log_d("getTmpDev: success dev sn = %s", (*it)->getSn().c_str());
            return *it;
        }
    }

    return NULL;
}

bool SubDeviceManager::creatSubDev(SubDevice *tmpDev)
{
    Lock lock(mutex_m);
    Db db;
    uint32_t nid;
    uint16_t zid;

    if(!db.randomNid(nid)){
        log_d("creatSubDev: randomNid failed");
        return false;
    }

    if(!db.randomZid(zid)){
        log_d("creatSubDev: randomZid failed");
        return false;
    }

    db.registDev(nid, zid, tmpDev->getSn());
    SubDevice *dev = new SubDevice(tmpDev->getSn(), tmpDev->getVer());
    dev->setParam(getFreeZid(dev->getSn()), getFreeNid(dev->getSn()));
    subDeviceList_m.push_back(dev);
    log_d("creatSubDev: success dev sn = %s", dev->getSn().c_str());
    return true;
}

bool SubDeviceManager::creatSubDev(SubDevice *tmpDev, uint16_t staticZid)
{
    Lock lock(mutex_m);

    Db db;
    uint32_t nid;

    if(!db.randomNid(nid)){
        log_d("creatSubDev: randomNid failed");
        return false;
    }

    db.registDev(nid, staticZid, tmpDev->getSn());
    SubDevice *dev = new SubDevice(tmpDev->getSn(), tmpDev->getVer());
    dev->setParam(staticZid, getFreeNid(dev->getSn()));
    subDeviceList_m.push_back(dev);
    log_d("creatSubDev: success dev sn = %s", dev->getSn().c_str());
    return true;
}


string SubDeviceManager::getSubDevName(uint32_t nid)
{
    Lock lock(mutex_m);
    for(vector<SubDevice *>::iterator it = subDeviceList_m.begin(); it != subDeviceList_m.end(); ++it){
        if(nid == (*it)->getNid()){
            return (*it)->getName();
        }
    }

    return "";
}

bool SubDeviceManager::creatTmpDev(const string &devSn, uint16_t protVer, uint16_t zid)
{
    Lock lock(mutex_m);
    Db db;
    if(!db.isLegalDev(devSn)){
        return false;
    }

    SubDevice *dev = new SubDevice(devSn, protVer, zid);
    dev->setParam(zid,0);
    tmpDevList_m.push_back(dev);
    return true;
}

// bool SubDeviceManager::creatTmpDev(const vector<uint8_t> &devSn, uint16_t protVer)
// {
//     Lock lock(mutex_m);
//     Db db;
//     if(!db.isLegalDev(devSn)){
//         return false;
//     }

//     SubDevice *dev = new SubDevice(sn, protVer);
//     dev->setParam(1,0);
//     tmpDevList_m.push_back(dev);
//     return true;
// }

void SubDeviceManager::clearTmpDev()
{
    Lock lock(mutex_m);
    for(vector<SubDevice *>::iterator it = tmpDevList_m.begin(); it != tmpDevList_m.end(); ++it){
        delete *it;
    }
    tmpDevList_m.clear();
}

void SubDeviceManager::clearSubDev()
{
    Lock lock(mutex_m);
    for(vector<SubDevice *>::iterator it = subDeviceList_m.begin(); it != subDeviceList_m.end(); ++it){
        delete *it;
    }
    subDeviceList_m.clear();
}


/* ----- End of file ----- */
