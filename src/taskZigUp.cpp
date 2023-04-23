#include "taskZigUp.h"
#include "msgQueue.h"
#include "zigbee.h"

void *taskZigUp(void *arg)
{
    for(;;){
        int len = 0;
        vector<uint8_t> msg;
        DevMsgRecvQueue &recvQ = DevMsgRecvQueue::getInstance();
        len = recvQ.waitMsg(msg);
        if(0 == len){
            continue;
        }

        ZigMsg_t *pZigMsg = (ZigMsg_t *)msg.data();
        vector<uint8_t> data;
        data.insert(data.end(), pZigMsg->data, pZigMsg->data + 16);
        Log::print("zigbee recv msg: ", msg);
        Log::print("zigbee recv data: ", data);
        log_i("zid = %04x: ", pZigMsg->netLayer.zid);
    }
}

/* ----- End of file ----- */
