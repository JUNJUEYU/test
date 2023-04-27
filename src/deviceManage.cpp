#include "deviceManage.h"

string DevManager::getGwPd(){
    return "TG-10_N";
}

string DevManager::getGwSn(){
    return "TG-10_N20230423";
}

DevManager::DevManager()
{
    topic_m = new TopicManager;
}

DevManager &DevManager::getInstance()
{
    static DevManager dev;
    return dev;
}

TopicManager *DevManager::getTopic()
{
    return topic_m;
}

#define TOPIC_SUB_PRO_SET "/cloud/gw/1/property/set"
#define TOPIC_SUB_PRO_GET "/cloud/gw/1/property/get"
#define TOPIC_SUB_CMD     "/cloud/gw/1/command"
#define TOPIC_SUB_EVT_RSP     "/cloud/gw/1/event/response"

#define TOPIC_PUB_PRO_SET_RSP "/cloud/gw/1/property/set/response"
#define TOPIC_PUB_PRO_GET_RSP "/cloud/gw/1/property/get/response"
#define TOPIC_PUB_CMD_RSP     "/cloud/gw/1/command/response"
#define TOPIC_PUB_EVT_RPT     "/cloud/gw/1/event/report"

string TopicManager::pubCmdRsp()
{
    return TOPIC_PUB_CMD_RSP;
}

string TopicManager::pubEvtRpt()
{
    return TOPIC_PUB_EVT_RPT;
}

string TopicManager::pubProGetRsp()
{
    return TOPIC_PUB_PRO_GET_RSP;
}

string TopicManager::pubProSetRsp()
{
    return TOPIC_PUB_PRO_SET_RSP;
}

string TopicManager::subCmd()
{
    return TOPIC_SUB_CMD;
}

string TopicManager::subEvtRsp()
{
    return TOPIC_SUB_EVT_RSP;
}

string TopicManager::subProGet()
{
    return TOPIC_SUB_PRO_GET;
}

string TopicManager::subProSet()
{
    return TOPIC_SUB_PRO_SET;
}

