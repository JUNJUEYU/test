#include "taskDevMsgProcess.h"
#include "log.h"
#include "msgQueue.h"
#include "zigProtStack.h"
#include "subDevManager.h"
#include "threadPool.h"
#include "eventTS30.h"
#include "eventUniv.h"
#include "task.h"
#include <vector>
#include <stdint.h>

typedef struct ProcessPair {
        string name;
        Event *(*process)(zigProtStack &);
}ProcessPair_t;


static Event *ts30Process(zigProtStack &prot)
{
    SubDeviceManager &devManager =  SubDeviceManager::getInstance();
    TS30Event &ts30 = TS30Event::getInstance();
    log_d("ts30Process");
    Event *pEvent = ts30.createEvent(devManager.getSubDev(prot.getSrcNid()),
                                    prot.getType(),
                                    prot.getMsgID(),
                                    prot.getData());

    return pEvent;
}

static Event *netAccessProcess(zigProtStack &prot, const string &sn)
{
    log_d("netAccessProcess sn = %s", sn.c_str());
    SubDeviceManager &devManager =  SubDeviceManager::getInstance();
    UnivEvent &univ = UnivEvent::getInstance();
    SubDevice *pDev = devManager.getTmpDev(sn);
    Event *pEvent = univ.createEvent(devManager.getTmpDev(sn),
                                    prot.getType(),
                                    prot.getMsgID(),
                                    prot.getData());

    return pEvent;
}


static ProcessPair_t processMap[] = {
    {"TS30", ts30Process}
};

void *taskDevMsgProcess(void *arg)
{
    for(;;){
        vector<uint8_t> msg;
        int len = DevMsgRecvQueue::getInstance().waitMsg(msg);
        if(0 == len){
            log_d("wait msg from dev error len = 0");
            continue;
        }
        log_d("recv msg from dev len = %d", len);

        zigProtStack protocol(msg);
        if(!protocol.parse()){
            log_d("protocol parse error");
            continue;
        }

        SubDeviceManager &devManager =  SubDeviceManager::getInstance();
        string ret = devManager.verify(protocol.getSrcNid(), protocol.getTarNid());
        log_d("verify ret = %s", ret.c_str());
        if(ret == "illegal"){
            log_d("verify illegal");
            continue;
        }
        else if(ret == "netAccess"){
            string sn = Trans::toString(protocol.getData(), 0, 14);
            devManager.creatTmpDev(sn, protocol.getProtVer(), protocol.getZid());
            Event *pEvent = netAccessProcess(protocol, sn);
            ThreadPool::getInstance().addTask(task, pEvent);
            continue;
        }

        for(int i = 0; i < sizeof(processMap)/sizeof(ProcessPair_t); i++){
            SubDevice *dev = devManager.getSubDev(protocol.getSrcNid());
            if(NULL == dev){
                log_d("get sub dev failed");
                break;
            }
            
            if(processMap[i].name == dev->getName()){
                Event *pEvent = processMap[i].process(protocol);
                if (NULL == pEvent) {
                    log_d("create event failed");
                    continue;
                }
                log_d("create event success");
                ThreadPool::getInstance().addTask(task, pEvent);
                break;
            }
        }
    }
}

/* ----- End of file ----- */
