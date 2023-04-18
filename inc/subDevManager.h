#pragma once
#include "plugin.h"
#include "idPool.h"
#include <vector>
#include <pthread.h>
#include "log.h"

using namespace std;

#define NET_ACCESS "Network Access"
#define DEV_ILLEGAL "Device Illegal"
#define DEV_LEGAL "Device Legal"


class Topology {
    public:
        Topology();
        ~Topology();
        bool isPermised(const string &){
            return true;
        };
    private:
        vector<string> devSnList_m;
};

class SubDevice {
    public:
        SubDevice(const string &sn, uint16_t ver);
        SubDevice(const string &sn, uint16_t ver, uint16_t zid);
        ~SubDevice(){};
        void setParam(uint16_t zid, uint32_t nid);
        uint32_t getNid(){return nid_m;};
        uint16_t getZid(){return zid_m;};
        string getSn(){
            return sn_m;};
        uint16_t getVer(){return ver_m;};
        string getName(){return name_m;};

        void setSurvivalTime(uint32_t time){survivalTime_m = time;};
        void setLastHeartTimeStamp(uint32_t time){lastHeartTimeStamp_m = time;};

    private:
        SubDevice(const SubDevice &);
        void operator=(const SubDevice &){};
        string sn_m;
        uint16_t zid_m;
        uint32_t nid_m;
        uint32_t survivalTime_m;
        uint32_t lastHeartTimeStamp_m;
        uint16_t ver_m;
        string name_m;
};

//协议管理
class SubDeviceManager {
    public:
        static SubDeviceManager& getInstance();
        bool creatTmpDev(const string &devSn, uint16_t protVer, uint16_t zid);
        // bool creatTmpDev(const vector<uint8_t> &devSn, uint16_t protVer);
        bool creatSubDev(SubDevice *tmpDev);
        bool creatSubDev(SubDevice *tmpDev, uint16_t staitcZid);
        string verify(uint32_t srcNid, uint32_t tarNid);

        SubDevice *getSubDev(uint32_t nid);
        SubDevice *getSubDev(const string &sn);
        SubDevice *getTmpDev(const string &sn);
        string getSubDevName(uint32_t nid);
        void deleteTmpDev(const string &sn);
        void clearTmpDev();
        void clearSubDev();

        void printSubDevList(){
            log_d("SubDeviceManager subDevice count = ", subDeviceList_m.size());
        };

        void printTmpDevList(){
            log_d("SubDeviceManager tmpDevice count = ", tmpDevList_m.size());
        };

    private:
        SubDeviceManager(SubDeviceManager const&);
        void operator=(SubDeviceManager const&){};
        SubDeviceManager();
        ~SubDeviceManager(){};

        uint16_t getFreeZid(const string &);
        uint32_t getFreeNid(const string &);

        void lock();
        void unlock();

    private:
        PluginManager *pluginManager_m;
        Topology *topology_m;
        vector<SubDevice *> subDeviceList_m;
        vector<SubDevice *> tmpDevList_m;
        IdPool *idPool_m;
        pthread_mutex_t mutex_m;
};

/* ----- End of file ----- */
