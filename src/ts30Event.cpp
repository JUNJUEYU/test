#include "ts30Event.h"

class TS30EventTest1 : public Event
{
public:
    TS30EventTest1(SubDevice *dev ,const string &cmd):Event(dev, cmd)
    {};
    ~TS30EventTest1(){
        cout << "TS30EventTest1 destructor" << endl;
    };
    uint8_t run(){cout << getNetData() << endl;};
};

class TS30EventTest2 : public Event
{
 public:
    TS30EventTest2(SubDevice *dev ,const string &cmd):Event(dev, cmd)
    {};
    ~TS30EventTest2(){
        cout << "TS30EventTest2 destructor" << endl;
    };
    uint8_t run(){cout << getNetData() << endl;}; 
};

class TS30EventTest3 : public Event
{
public:
    TS30EventTest3(SubDevice *dev ,const string &cmd):Event(dev, cmd)
    {};
    ~TS30EventTest3(){
        cout << "TS30EventTest3 destructor" << endl;
    };
    uint8_t run(){cout << getNetData() << endl;};
};

class TS30EventTest4 : public Event
{
public:
    TS30EventTest4(SubDevice *dev ,const string &cmd):Event(dev, cmd)
    {};
    ~TS30EventTest4(){
        cout << "TS30EventTest4 destructor" << endl;
    };
    uint8_t run(){cout << getNetData() << endl;};
};

class TS30EventTest5 : public Event
{
public:
    TS30EventTest5(SubDevice *dev ,const string &cmd):Event(dev, cmd)
    {};
    ~TS30EventTest5(){
        cout << "TS30EventTest5 destructor" << endl;
    };
    uint8_t run(){cout << getNetData() << endl;};
};


class TS30EventAccessDynamic : public Event
{
public:
    TS30EventAccessDynamic(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventAccessDynamic(){};
    uint8_t run(){};
};

class TS30EventAccessStatic : public Event
{
public:
    TS30EventAccessStatic(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventAccessStatic(){};
    uint8_t run(){};
};

class TS30EventHeartBeat : public Event
{
public:
    TS30EventHeartBeat(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventHeartBeat(){};
    uint8_t run(){};
};

class TS30EventSignal : public Event
{
public:
    TS30EventSignal(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventSignal(){};
    uint8_t run(){};
};

class TS30EventFault : public Event
{
public:
    TS30EventFault(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventFault(){};
    uint8_t run(){};
};

class TS30EventTimeSync : public Event
{
public:
    TS30EventTimeSync(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventTimeSync(){};
    uint8_t run(){};
};

class TS30EventOtaReady : public Event
{
public:
    TS30EventOtaReady(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventOtaReady(){};
    uint8_t run(){};
};

class TS30EventOtaAck : public Event
{
public:
    TS30EventOtaAck(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventOtaAck(){};
    uint8_t run(){};
};

class TS30EventRestartAck : public Event
{
public:
    TS30EventRestartAck(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventRestartAck(){};
    uint8_t run(){};
};

class TS30EventEnterConfigAck : public Event
{
public:
    TS30EventEnterConfigAck(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventEnterConfigAck(){};
    uint8_t run(){};
};

class TS30EventExitConfigAck : public Event
{
public:
    TS30EventExitConfigAck(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventExitConfigAck(){};
    uint8_t run(){};
};

class TS30EventConfigAck : public Event
{
public:
    TS30EventConfigAck(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventConfigAck(){};
    uint8_t run(){};
};

class TS30EventConfig : public Event
{
public:
    TS30EventConfig(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventConfig(){};
    uint8_t run(){};
};

class TS30EventParamNormalAck : public Event
{
public:
    TS30EventParamNormalAck(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventParamNormalAck(){};
    uint8_t run(){};
};

class TS30EventParamZigbeeAck : public Event
{
    public:
    TS30EventParamZigbeeAck(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventParamZigbeeAck(){};
    uint8_t run(){};
};


class TS30EventParamLogAck : public Event
{
public:
    TS30EventParamLogAck(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventParamLogAck(){};
    uint8_t run(){};
};

class TS30EventParamReportIntervalAck : public Event
{
public:
    TS30EventParamReportIntervalAck(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventParamReportIntervalAck(){};
    uint8_t run(){};
};

class TS30EventParamArtificialAck : public Event
{
public:
    TS30EventParamArtificialAck(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventParamArtificialAck(){};
    uint8_t run(){};
};

class TS30EventParamDigitalAck : public Event
{
public:
    TS30EventParamDigitalAck(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventParamDigitalAck(){};
    uint8_t run(){};
};

class TS30EventParamDigitalOutAck : public Event
{
public:
    TS30EventParamDigitalOutAck(SubDevice *dev ,const vector<uint8_t> &data):Event(dev, data)
    {};
    ~TS30EventParamDigitalOutAck(){};
    uint8_t run(){};
};

static Event *creatEventAccessDynamic(SubDevice *dev, const vector<uint8_t> &data)
{
    return new TS30EventAccessDynamic(dev, data);
}

static Event *creatEventAccessStatic(SubDevice *dev, const vector<uint8_t> &data)
{
    return new TS30EventAccessStatic(dev, data);
}

static Event *creatEventHeartBeat(SubDevice *dev, const vector<uint8_t> &data)
{
    return new TS30EventHeartBeat(dev, data);
}

static Event *creatEventSignal(SubDevice *dev, const vector<uint8_t> &data)
{
    return new TS30EventSignal(dev, data);
}

static Event *creatEventFault(SubDevice *dev, const vector<uint8_t> &data)
{
    return new TS30EventFault(dev, data);
}

static Event *creatEventTimeSync(SubDevice *dev, const vector<uint8_t> &data)
{
    return new TS30EventTimeSync(dev, data);
}

static Event *creatEventOta(SubDevice *dev, const vector<uint8_t> &data)
{
    return new TS30EventOtaReady(dev, data);
}

static Event *creatEventOtaAck(SubDevice *dev, const vector<uint8_t> &data)
{
    return new TS30EventOtaAck(dev, data);
}

static Event *creatEventRestartAck(SubDevice *dev, const vector<uint8_t> &data)
{
    return new TS30EventRestartAck(dev, data);
}

static Event *creatEventEnterConfigAck(SubDevice *dev, const vector<uint8_t> &data)
{
    return new TS30EventEnterConfigAck(dev, data);
}

static Event *creatEventExitConfigAck(SubDevice *dev, const vector<uint8_t> &data)
{
    return new TS30EventExitConfigAck(dev, data);
}

static Event *creatEventParamNormalAck(SubDevice *dev, const vector<uint8_t> &data)
{
    return new TS30EventParamNormalAck(dev, data);
}

static Event *creatEventParamZigbeeAck(SubDevice *dev, const vector<uint8_t> &data)
{
    return new TS30EventParamZigbeeAck(dev, data);
}

static Event *creatEventParamLogAck(SubDevice *dev, const vector<uint8_t> &data)
{
    return new TS30EventParamLogAck(dev, data);
}

static Event *creatEventParamReportIntervalAck(SubDevice *dev, const vector<uint8_t> &data)
{
    return new TS30EventParamReportIntervalAck(dev, data);
}

static Event *creatEventParamArtificialAck(SubDevice *dev, const vector<uint8_t> &data)
{
    return new TS30EventParamArtificialAck(dev, data);
}

static Event *creatEventParamDigitalAck(SubDevice *dev, const vector<uint8_t> &data)
{
    return new TS30EventParamDigitalAck(dev, data);
}

static Event *creatEventParamDigitalOutAck(SubDevice *dev, const vector<uint8_t> &data)
{
    return new TS30EventParamDigitalOutAck(dev, data);
}

static Event *creatEventTest1(SubDevice *dev, const string &data)
{
    return new TS30EventTest1(dev, data);
}

static Event *creatEventTest2(SubDevice *dev, const string &data)
{
    return new TS30EventTest2(dev, data);
}

static Event *creatEventTest3(SubDevice *dev, const string &data)
{
    return new TS30EventTest3(dev, data);
}

static Event *creatEventTest4(SubDevice *dev, const string &data)
{
    return new TS30EventTest4(dev, data);
}

static Event *creatEventTest5(SubDevice *dev, const string &data)
{
    return new TS30EventTest5(dev, data);
}



void TS30Event::init(){
    devEventMap_m.insert(pair<uint16_t, DevEventFunc_t>(0x0101, creatEventAccessDynamic));
    devEventMap_m.insert(pair<uint16_t, DevEventFunc_t>(0x0102, creatEventAccessStatic));
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

    netEventMap_m.insert(pair<string, NetEventFunc_t>("test1", creatEventTest1));
    netEventMap_m.insert(pair<string, NetEventFunc_t>("test2", creatEventTest2));
    netEventMap_m.insert(pair<string, NetEventFunc_t>("test3", creatEventTest3));
    netEventMap_m.insert(pair<string, NetEventFunc_t>("test4", creatEventTest4));
    netEventMap_m.insert(pair<string, NetEventFunc_t>("test5", creatEventTest5));
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

Event *TS30Event::createEvent(SubDevice *dev,const string &netData)
{
    string cmd = getNetDataCmd(netData);

    map<string, NetEventFunc_t>::iterator it = netEventMap_m.find(cmd);
    if (it != netEventMap_m.end())
    {
        return it->second(dev, netData);
    }
    return NULL;
}

Event *TS30Event::createEvent(SubDevice *dev,const vector<uint8_t> &devData)
{
    uint16_t key = (devData[0] << 8) | devData[1];
    map<uint16_t, DevEventFunc_t>::iterator it = devEventMap_m.find(key);
    if (it != devEventMap_m.end())
    {
        return it->second(dev, devData);
    }
    return NULL;
}

/* ----- End of file ----- */