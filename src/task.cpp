#include "task.h"
#include <memory>
#include "msgQueue.h"


void *task(void *arg)
{
    for(;;){
        vector<uint8_t> msg;
        DevMsgRecvQueue::getInstance().waitMsg(msg);
        
    }
}

/* ----- End of file ----- */
