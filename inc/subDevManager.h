#pragma once
#include "plugin.h"
#include "idPool.h"
#include <vector>
//mutex c++03 without boost
#include <pthread.h>

using namespace std;

#define NET_ACCESS "Network Access"
#define DEV_ILLEGAL "Device Illegal"
#define DEV_LEGAL "Device Legal"


class Topology {
    public:
        Topology();
        ~Topology();
        bool isPermised(const string &){

        };
    private:
        vector<string> devSnList_m;
};

class SubDevice {
    public:
        SubDevice(const string &sn, ProtAPI *port):
            pProt_m(port),
            sn_m(sn){};
        ~SubDevice(){
            if(NULL != pProt_m){
                delete pProt_m;
            }
        };
        void setParam(uint16_t zid, uint32_t nid);
        uint32_t getNid(){return nid_m;};
        uint16_t getZid(){return zid_m;};
        string getSn(){return sn_m;};
        uint16_t getVer(){return pProt_m->getVersion();};

    private:
        SubDevice(const SubDevice &);
        void operator=(const SubDevice &){};
        ProtAPI *pProt_m;
        string sn_m;
        uint16_t zid_m;
        uint32_t nid_m;
        uint32_t survivalTime_m;
        uint32_t lastHeartTimeStamp_m;
};

//协议管理
class SubDeviceManager {
    public:
        static SubDeviceManager& getInstance();
        bool creatSubDev(const string &devSn, uint16_t protVer);
        bool creatSubDev(const string &devSn, uint16_t protVer, uint16_t zid);
        bool verify(uint32_t);
        uint16_t getZid(const string &devSn);
        uint32_t getNid(const string &devSn);
        uint16_t getVer(const string &devSn);
        uint16_t getVer(uint32_t &devNid);

    private:
        SubDeviceManager(SubDeviceManager const&);
        void operator=(SubDeviceManager const&){} ;
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
        IdPool *idPool_m;
        pthread_mutex_t mutex_m;
};

/* ----- End of file ----- */
