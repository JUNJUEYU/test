#include "eventUniv.h"
#include "eventManager.h"
#include "zigProtStack.h"
#include "cJSON.h"


class UnivEventNetAccDynamic : public Event
{
    class Ack{
    public:
        Ack(uint32_t nid, uint16_t zid):nid_m(nid), zid_m(zid)
        {
        };
        ~Ack(){};
        vector<uint8_t> getAck(){
            vector<uint8_t> msg;
            msg.push_back(0);
            msg.push_back(nid_m & 0xff);
            msg.push_back((nid_m >> 8) & 0xff);
            msg.push_back((nid_m >> 16) & 0xff);
            msg.push_back((nid_m >> 24) & 0xff);
            msg.push_back(zid_m & 0xff);
            msg.push_back((zid_m >> 8) & 0xff);
            return msg;
        }
    private:
        uint32_t nid_m;
        uint16_t zid_m;
    };

public:
    UnivEventNetAccDynamic(SubDevice *dev ,uint16_t msgID, const vector<uint8_t> &data):Event(dev,msgID, data)
    {
    };
    ~UnivEventNetAccDynamic(){
    };
    uint8_t run(){
        /* Get tmp-dev sn */
        string sn = Trans::toString(devData_m, 0, 14);
        SubDeviceManager &subDevManager = SubDeviceManager::getInstance();
        /* Creat a dev by tmp-dev */
        subDevManager.creatSubDev(dev_m);
        SubDevice *dev = subDevManager.getSubDev(sn);
        if(dev == NULL){
            log_d("UnivEventNetAccDynamic::run:  dev == NULL");
            return 0;}
        Ack ack(dev->getNid(), dev->getZid());

        /* Pack ack msg */
        zigProtStack prot(ack.getAck());
        prot.setProtVer(dev_m->getVer());
        prot.setSrcNid(1);
        prot.setTarNid(dev_m->getNid());
        prot.setType(0x0101);
        prot.setMsgID(getMsgID());
        prot.setZid(dev_m->getZid());
        prot.pack();

        /* Delete tmp-dev & Send ack */
        subDevManager.deleteTmpDev(sn);
        DevMsgSendQueue::getInstance().pushMsg(prot.getData());
    };
};

class UnivEventNetAccStatic: public Event
{
    class Ack{
    public:
        Ack(uint32_t nid):nid_m(nid)
        {
        };
        ~Ack(){};
        vector<uint8_t> getAck(){
            vector<uint8_t> msg;
            msg.push_back(0);
            msg.push_back(nid_m & 0xff);
            msg.push_back((nid_m >> 8) & 0xff);
            msg.push_back((nid_m >> 16) & 0xff);
            msg.push_back((nid_m >> 24) & 0xff);
            return msg;
        }
    private:
        uint32_t nid_m;
    };

public:
    UnivEventNetAccStatic(SubDevice *dev ,uint16_t msgID, const vector<uint8_t> &data):Event(dev,msgID,data)
    {};
    ~UnivEventNetAccStatic(){};
    uint8_t run(){
        /* Get tmp-dev sn */
        string sn = Trans::toString(devData_m, 0, 14);
        uint16_t staticZid = devData_m[20] << 8 | devData_m[21];
        SubDeviceManager &subDevManager = SubDeviceManager::getInstance();
        /* Creat a dev by tmp-dev */
        subDevManager.creatSubDev(dev_m, staticZid);
        SubDevice *dev = subDevManager.getSubDev(sn);
        if(dev == NULL){
            log_d("UnivEventNetAccStatic::run:  dev == NULL");
            return 0;}
        Ack ack(dev->getNid());

        /* Pack ack msg */
        zigProtStack prot(ack.getAck());
        prot.setProtVer(dev_m->getVer());
        prot.setSrcNid(1);
        prot.setTarNid(dev_m->getNid());
        prot.setType(0x0102);
        prot.setMsgID(getMsgID());
        prot.setZid(dev_m->getZid());
        prot.pack();
        /* Delete tmp-dev & Send ack */
        subDevManager.deleteTmpDev(sn);
        DevMsgSendQueue::getInstance().pushMsg(prot.getData());
        log_d("UnivEventNetAccStatic::run:  send ack");
    };
};


class UnivEventDataAck: public Event
{
public:
    UnivEventDataAck(const string &topic, const string &data):Event(topic,data)
    {};
    ~UnivEventDataAck(){};
    uint8_t run(){
        cJSON *root = cJSON_Parse(netData_m.c_str());
        if(root == NULL){
            log_d("UnivEventDataAck::run:  root == NULL");
            return 0;
        }
        cJSON *msgID = cJSON_GetObjectItem(root, "msg_id");
        if(msgID == NULL){
            log_d("UnivEventDataAck::run:  msgID == NULL");
            return 0;
        }
        cJSON *result = cJSON_GetObjectItem(root, "result");
        if(result == NULL){
            log_d("UnivEventDataAck::run:  result == NULL");
            return 0;
        }

        EventManager &eMana = EventManager::getInstance();
        Event *event = eMana.getPendEvent(msgID->valuestring);
        if(event == NULL){
            log_d("UnivEventDataAck::run:  event == NULL");
            return 0;
        }
        event->pushMsg(result->valuestring);

        log_d("UnivEventDataAck::run");
    };
};

static Event *creatEventNetAccessDynamic(SubDevice *dev, uint16_t msgID, const vector<uint8_t> &msg)
{
    log_d("creatEventNetAccessDynamic");
    return new UnivEventNetAccDynamic(dev, msgID, msg);
}

static Event *creatEventNetAccessStatic(SubDevice *dev, uint16_t msgID, const vector<uint8_t> &msg)
{
    log_d("creatEventNetAccessStatic");
    return new UnivEventNetAccStatic(dev, msgID, msg);
}

static Event *creatEventDataAck(const string &topic, const string &msg)
{
    log_d("creatEventDataAck");
    return new UnivEventDataAck(topic, msg);
}

UnivEvent::UnivEvent()
{
    univEventMap_m.insert(pair<uint16_t, UinvEventFunc_t>(0x0101, creatEventNetAccessDynamic));
    univEventMap_m.insert(pair<uint16_t, UinvEventFunc_t>(0x0102, creatEventNetAccessStatic));
    netEventMap_m.insert(pair<string, NetEventFunc_t>("topic1", creatEventDataAck));
}

UnivEvent::~UnivEvent()
{;}


Event *UnivEvent::createEvent(const string &topic,const string &netData)
{
    map<string, NetEventFunc_t>::iterator it = netEventMap_m.find(topic);
    if (it != netEventMap_m.end())
    {
        return it->second(topic, netData);
    }
    return NULL;
}

Event *UnivEvent::createEvent(SubDevice *dev, uint16_t type, uint16_t msgID, const vector<uint8_t> &devData)
{
    map<uint16_t, UinvEventFunc_t>::iterator it = univEventMap_m.find(type);
    if (it != univEventMap_m.end())
    {
        return it->second(dev, msgID, devData);
    }
    return NULL;
}

/* ----- End of file ----- */

