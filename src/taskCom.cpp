#include "taskCom.h"
#include "log.h"
#include "serialPort.h"
#include "msgQueue.h"
#include <sys/epoll.h>
#include <unistd.h>

SerialPort *pSerialPort_g = NULL;

static void comInit(void)
{
    if(NULL != pSerialPort_g){
        return;
    }
    pSerialPort_g = new SerialPort;
    pSerialPort_g->openPort("/dev/ttymxc5");
    pSerialPort_g->setOpt(115200, 8, 'N', 1);
}


void *taskComRead(void *arg)
{
    /* Init Com */
    comInit();

    /* Blocking read */
    for(;;){
        if(!pSerialPort_g->waitData()){
            log_d("wait data error");
            continue;
        }
        log_d("wait data success");
        
        vector<uint8_t> data;
        pSerialPort_g->readData(data);
        DevMsgRecvQueue::getInstance().pushMsg(data); 
    }
}


void *taskComSend(void *arg)
{
    /* Init Com */
    comInit();

    /* Blocking send */
    for(;;){
        DevMsgSendQueue &devMsgQ = DevMsgSendQueue::getInstance();
        vector<uint8_t> msg;

        if(0 == devMsgQ.waitMsg(msg)){
            log_d("send msg length = 0");
            continue;
        }

        pSerialPort_g->writeData(msg);
    }
}

/* ----- End of file ----- */
