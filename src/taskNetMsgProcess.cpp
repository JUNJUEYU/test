#include "taskNetMsgProcess.h"
#include "log.h"
#include "msgQueue.h"
#include "zigProtStack.h"
#include "subDevManager.h"
#include "threadPool.h"
#include "eventTS30.h"
#include "eventUniv.h"
#include "mqttProtStack.h"
#include "eventManager.h"
#include "task.h"
#include <vector>
#include <stdint.h>


void *taskNetMsgProcess(void *arg)
{
    for(;;){
        string topic, msg;
        uint32_t len = NetMsgRecvQueue::getInstance().waitMsg(topic, msg);
        if(0 == len){
            log_d("wait msg from net error len = 0");
            continue;
        }

        UnivEvent &univ = UnivEvent::getInstance();
        Event *pEvent = univ.createEvent(topic, msg);
        if(NULL == pEvent){
            log_d("create event failed");
            continue;
        }

        ThreadPool::getInstance().addTask(task, pEvent);
    }
}

/* ----- End of file ----- */
