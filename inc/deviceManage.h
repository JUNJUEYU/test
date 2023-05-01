#pragma once
#include <string>
#include <stdint.h>
using namespace std;

class TopicManager{
    public:
        string subProSet();
        string subProGet();
        string subCmd();
        string subEvtRsp();
        string pubProSetRsp();
        string pubProGetRsp();
        string pubCmdRsp();
        string pubEvtRpt();
};

class DevManager{
    public:
        string getGwPd();
        string getGwSn();
        string getDevPd(uint32_t nid);
        string getDevSn(uint32_t nid);
        bool isInDevList(uint32_t nid);
        TopicManager *getTopic();
        static DevManager &getInstance();
    private:
        DevManager();
        DevManager &operator=(const DevManager &){};
        DevManager(const DevManager &){};
        TopicManager *topic_m;
};