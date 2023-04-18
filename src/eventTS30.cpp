#include "eventTS30.h"
#include "mqttMsgTS30.h"
#include "zigProtStack.h"

class AckBase
{
public:
    vector<uint8_t> getAck(){
        return data_m;
    }
    virtual ~AckBase(){
    };
    protected:
        vector<uint8_t> data_m;
};



class TS30EventHeartBeat : public Event
{
    class Ack:public AckBase{
    public:
        Ack(uint8_t result){
            data_m.push_back(result);
        };
    };

public:
    TS30EventHeartBeat(SubDevice *dev , uint16_t msgID, const vector<uint8_t> &data):Event(dev, msgID, data)
    {};
    ~TS30EventHeartBeat(){};
    uint8_t run(){
        uint32_t time = devData_m[3] << 24 | devData_m[2] << 16 | devData_m[1] << 8 | devData_m[0];
        uint8_t logLen = devData_m[4];
        dev_m->setSurvivalTime(time);
        dev_m->setLastHeartTimeStamp(UtcTime::getUtcTimeStamp());
        if(logLen > 0){
            string log = Trans::toString(devData_m,5, logLen);
            saveDevLog(log);
        }

        Ack ack(0);
        zigProtStack prot(ack.getAck());
        prot.setParam(dev_m->getZid(),dev_m->getVer(),1,dev_m->getNid(),0x0201,getMsgID());
        DevMsgSendQueue::getInstance().pushMsg(prot.getData());
    };
private:
    void saveDevLog(const string &log){};
};

class TS30EventSignal : public Event
{
    class Ack:public AckBase{
    public:
        void setResult(uint8_t result){
            data_m.clear();
            data_m.push_back(result);
        };
    };
public:
    TS30EventSignal(SubDevice *dev , uint16_t msgID, const vector<uint8_t> &data):Event(dev, msgID, data)
    {};
    ~TS30EventSignal(){};
    uint8_t run(){
        log_d("TS30EventSignal run");
        MqttMsgDataUpload mqttMsg(devData_m);
        NetMsgSendQueue &netMsgQueue = NetMsgSendQueue::getInstance();
        netMsgQueue.pushMsg(mqttMsg.getTopic(), mqttMsg.getMsg());

        string reponse;
        Ack ack;
        if(0 == waitResponse(reponse, 2)){
            log_d("TS30EventSignal waitMsg timeout");
            ack.setResult(1);
        }
        else{
            log_d("TS30EventSignal waitMsg success");
            ack.setResult(0);
        }

        zigProtStack prot(ack.getAck());
        prot.setParam(dev_m->getZid(),dev_m->getVer(),1,dev_m->getNid(),0x0202,getMsgID());
        DevMsgSendQueue::getInstance().pushMsg(prot.getData());
    };
};


class TS30EventFault : public Event
{
    class Ack:public AckBase{
    public:
        Ack(uint8_t result){
            data_m.push_back(result);
        };
    };

public:
    TS30EventFault(SubDevice *dev , uint16_t msgID, const vector<uint8_t> &data):Event(dev, msgID, data)
    {};
    ~TS30EventFault(){};
    uint8_t run(){
        MqttMsgFaultUpload mqttMsg(devData_m);
        NetMsgSendQueue &netMsgQueue = NetMsgSendQueue::getInstance();
        netMsgQueue.pushMsg(mqttMsg.getTopic(), mqttMsg.getMsg());

        Ack ack(0);
        zigProtStack prot(ack.getAck());
        prot.setParam(dev_m->getZid(),dev_m->getVer(),1,dev_m->getNid(),0x0203,getMsgID());
    };
};

class TS30EventTimeSync : public Event
{
    class Ack:public AckBase{
    public:
        Ack(uint8_t result, uint32_t time){
            data_m.push_back(result);
            data_m.push_back(time & 0xff);
            data_m.push_back((time >> 8) & 0xff);
            data_m.push_back((time >> 16) & 0xff);
            data_m.push_back((time >> 24) & 0xff);
        };
    };

public:
    TS30EventTimeSync(SubDevice *dev , uint16_t msgID, const vector<uint8_t> &data):Event(dev, msgID, data)
    {};
    ~TS30EventTimeSync(){};
    uint8_t run(){
        Ack ack(0, UtcTime::getUtcTimeStamp());
        zigProtStack prot(ack.getAck());
        prot.setParam(dev_m->getZid(),dev_m->getVer(),1,dev_m->getNid(),0x0204,getMsgID());
        DevMsgSendQueue::getInstance().pushMsg(prot.getData());
    };
};

class TS30EventOtaReady : public Event
{
public:
    TS30EventOtaReady(SubDevice *dev , uint16_t msgID, const vector<uint8_t> &data):Event(dev, msgID, data)
    {};
    ~TS30EventOtaReady(){};
    uint8_t run(){};
};

class TS30EventOtaAck : public Event
{
public:
    TS30EventOtaAck(SubDevice *dev , uint16_t msgID, const vector<uint8_t> &data):Event(dev, msgID, data)
    {};
    ~TS30EventOtaAck(){};
    uint8_t run(){};
};

class TS30EventRestartAck : public Event
{
public:
    TS30EventRestartAck(SubDevice *dev , uint16_t msgID, const vector<uint8_t> &data):Event(dev, msgID, data)
    {};
    ~TS30EventRestartAck(){};
    uint8_t run(){};
};

class TS30EventEnterConfigAck : public Event
{
public:
    TS30EventEnterConfigAck(SubDevice *dev , uint16_t msgID, const vector<uint8_t> &data):Event(dev, msgID, data)
    {};
    ~TS30EventEnterConfigAck(){};
    uint8_t run(){};
};

class TS30EventExitConfigAck : public Event
{
public:
    TS30EventExitConfigAck(SubDevice *dev , uint16_t msgID, const vector<uint8_t> &data):Event(dev, msgID, data)
    {};
    ~TS30EventExitConfigAck(){};
    uint8_t run(){};
};

class TS30EventConfigAck : public Event
{
public:
    TS30EventConfigAck(SubDevice *dev , uint16_t msgID, const vector<uint8_t> &data):Event(dev, msgID, data)
    {};
    ~TS30EventConfigAck(){};
    uint8_t run(){};
};

class TS30EventConfig : public Event
{
public:
    TS30EventConfig(SubDevice *dev , uint16_t msgID, const vector<uint8_t> &data):Event(dev, msgID, data)
    {};
    ~TS30EventConfig(){};
    uint8_t run(){};
};

class TS30EventParamNormalAck : public Event
{
public:
    TS30EventParamNormalAck(SubDevice *dev , uint16_t msgID, const vector<uint8_t> &data):Event(dev, msgID, data)
    {};
    ~TS30EventParamNormalAck(){};
    uint8_t run(){};
};

class TS30EventParamZigbeeAck : public Event
{
    public:
    TS30EventParamZigbeeAck(SubDevice *dev , uint16_t msgID, const vector<uint8_t> &data):Event(dev, msgID, data)
    {};
    ~TS30EventParamZigbeeAck(){};
    uint8_t run(){};
};


class TS30EventParamLogAck : public Event
{
public:
    TS30EventParamLogAck(SubDevice *dev , uint16_t msgID, const vector<uint8_t> &data):Event(dev, msgID, data)
    {};
    ~TS30EventParamLogAck(){};
    uint8_t run(){};
};

class TS30EventParamReportIntervalAck : public Event
{
public:
    TS30EventParamReportIntervalAck(SubDevice *dev , uint16_t msgID, const vector<uint8_t> &data):Event(dev, msgID, data)
    {};
    ~TS30EventParamReportIntervalAck(){};
    uint8_t run(){};
};

class TS30EventParamArtificialAck : public Event
{
public:
    TS30EventParamArtificialAck(SubDevice *dev , uint16_t msgID, const vector<uint8_t> &data):Event(dev, msgID, data)
    {};
    ~TS30EventParamArtificialAck(){};
    uint8_t run(){};
};

class TS30EventParamDigitalAck : public Event
{
public:
    TS30EventParamDigitalAck(SubDevice *dev , uint16_t msgID, const vector<uint8_t> &data):Event(dev, msgID, data)
    {};
    ~TS30EventParamDigitalAck(){};
    uint8_t run(){};
};

class TS30EventParamDigitalOutAck : public Event
{
public:
    TS30EventParamDigitalOutAck(SubDevice *dev , uint16_t msgID, const vector<uint8_t> &data):Event(dev, msgID, data)
    {};
    ~TS30EventParamDigitalOutAck(){};
    uint8_t run(){};
};


class TS30EventDataAck : public Event
{
public:
    TS30EventDataAck(const string &topic, const string &data):Event(topic, data)
    {};
    ~TS30EventDataAck(){};
    uint8_t run(){};
};


static Event *creatEventHeartBeat(SubDevice *dev, uint16_t msgID, const vector<uint8_t> &data)
{
    log_d("creatEventHeartBeat");
    return new TS30EventHeartBeat(dev,msgID, data);
}

static Event *creatEventSignal(SubDevice *dev, uint16_t msgID, const vector<uint8_t> &data)
{
    return new TS30EventSignal(dev,msgID, data);
}

static Event *creatEventFault(SubDevice *dev, uint16_t msgID, const vector<uint8_t> &data)
{
    return new TS30EventFault(dev,msgID, data);
}

static Event *creatEventTimeSync(SubDevice *dev, uint16_t msgID, const vector<uint8_t> &data)
{
    return new TS30EventTimeSync(dev,msgID, data);
}

static Event *creatEventOta(SubDevice *dev, uint16_t msgID, const vector<uint8_t> &data)
{
    return new TS30EventOtaReady(dev,msgID, data);
}

static Event *creatEventOtaAck(SubDevice *dev, uint16_t msgID, const vector<uint8_t> &data)
{
    return new TS30EventOtaAck(dev,msgID, data);
}

static Event *creatEventRestartAck(SubDevice *dev, uint16_t msgID, const vector<uint8_t> &data)
{
    return new TS30EventRestartAck(dev,msgID, data);
}

static Event *creatEventEnterConfigAck(SubDevice *dev, uint16_t msgID, const vector<uint8_t> &data)
{
    return new TS30EventEnterConfigAck(dev,msgID, data);
}

static Event *creatEventExitConfigAck(SubDevice *dev, uint16_t msgID, const vector<uint8_t> &data)
{
    return new TS30EventExitConfigAck(dev,msgID, data);
}

static Event *creatEventParamNormalAck(SubDevice *dev, uint16_t msgID, const vector<uint8_t> &data)
{
    return new TS30EventParamNormalAck(dev,msgID, data);
}

static Event *creatEventParamZigbeeAck(SubDevice *dev, uint16_t msgID, const vector<uint8_t> &data)
{
    return new TS30EventParamZigbeeAck(dev,msgID, data);
}

static Event *creatEventParamLogAck(SubDevice *dev, uint16_t msgID, const vector<uint8_t> &data)
{
    return new TS30EventParamLogAck(dev,msgID, data);
}

static Event *creatEventParamReportIntervalAck(SubDevice *dev, uint16_t msgID, const vector<uint8_t> &data)
{
    return new TS30EventParamReportIntervalAck(dev,msgID, data);
}

static Event *creatEventParamArtificialAck(SubDevice *dev, uint16_t msgID, const vector<uint8_t> &data)
{
    return new TS30EventParamArtificialAck(dev,msgID, data);
}

static Event *creatEventParamDigitalAck(SubDevice *dev, uint16_t msgID, const vector<uint8_t> &data)
{
    return new TS30EventParamDigitalAck(dev,msgID, data);
}

static Event *creatEventParamDigitalOutAck(SubDevice *dev, uint16_t msgID, const vector<uint8_t> &data)
{
    return new TS30EventParamDigitalOutAck(dev,msgID, data);
}

static Event *creatEventDataAck(const string &topic, const string &data)
{
    return new TS30EventDataAck(topic, data);
}


void TS30Event::init(){
    devEventMap_m.insert(pair<uint16_t, DevEventFunc_t>(0x0201, creatEventHeartBeat));
    devEventMap_m.insert(pair<uint16_t, DevEventFunc_t>(0x0202, creatEventSignal));
    devEventMap_m.insert(pair<uint16_t, DevEventFunc_t>(0x0203, creatEventFault));
    devEventMap_m.insert(pair<uint16_t, DevEventFunc_t>(0x0301, creatEventTimeSync));
    devEventMap_m.insert(pair<uint16_t, DevEventFunc_t>(0x0401, creatEventOta));
    devEventMap_m.insert(pair<uint16_t, DevEventFunc_t>(0x0402, creatEventOtaAck));
    devEventMap_m.insert(pair<uint16_t, DevEventFunc_t>(0x0403, creatEventRestartAck));
    devEventMap_m.insert(pair<uint16_t, DevEventFunc_t>(0x0404, creatEventEnterConfigAck));
    devEventMap_m.insert(pair<uint16_t, DevEventFunc_t>(0x0405, creatEventExitConfigAck));
    devEventMap_m.insert(pair<uint16_t, DevEventFunc_t>(0x0501, creatEventParamNormalAck));
    devEventMap_m.insert(pair<uint16_t, DevEventFunc_t>(0x0502, creatEventParamZigbeeAck));
    devEventMap_m.insert(pair<uint16_t, DevEventFunc_t>(0x0503, creatEventParamLogAck));
    devEventMap_m.insert(pair<uint16_t, DevEventFunc_t>(0x0504, creatEventParamReportIntervalAck));
    devEventMap_m.insert(pair<uint16_t, DevEventFunc_t>(0x0505, creatEventParamArtificialAck));
    devEventMap_m.insert(pair<uint16_t, DevEventFunc_t>(0x0506, creatEventParamDigitalAck));
    devEventMap_m.insert(pair<uint16_t, DevEventFunc_t>(0x0507, creatEventParamDigitalOutAck));

}

TS30Event::TS30Event()
{
    init();
}


TS30Event::~TS30Event()
{
}

static string getNetDataCmd(const string &src)
{
    return string(src.begin(), src.begin() + 5);
}


Event *TS30Event::createEvent(SubDevice *dev, uint16_t type, uint16_t msgID, const vector<uint8_t> &devData)
{
    map<uint16_t, DevEventFunc_t>::iterator it = devEventMap_m.find(type);
    if (it != devEventMap_m.end())
    {
        return it->second(dev, msgID, devData);
    }
    return NULL;
}


/* ----- End of file ----- */