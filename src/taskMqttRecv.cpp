#include "taskMqttRecv.h"
#include "log.h"
#include "myMqtt.h"
#include "msgQueue.h"
#include "deviceManage.h"
#include "tableProperty.h"
#include "tableCommand.h"
#include "tableEvent.h"



static void procProSet(string &msg)
{
    Property pro;
    PropertyTable &proTable = PropertyTable::getInstance();


}

static void procProGet(string &msg)
{
    
}

static void procCmd(string &msg)
{
    
}

static void procEvtRsp(string &msg)
{
    
}


static void *recvfunc(string &topic, string &payload)
{
    NetMsgRecvQueue &recvQ = NetMsgRecvQueue::getInstance();
    recvQ.pushMsg(topic, payload);
    return NULL;
}

void *taskMqttRecv(void *arg)
{
    NetMsgRecvQueue &recvQ = NetMsgRecvQueue::getInstance();
    DevManager &dev = DevManager::getInstance();
    Mqtt &mqtt = Mqtt::getInstance();
    mqtt.setRecvCallback(recvfunc);
    mqtt.subscribe(dev.getTopic()->subProSet());
    mqtt.subscribe(dev.getTopic()->subProGet());
    mqtt.subscribe(dev.getTopic()->subCmd());
    mqtt.subscribe(dev.getTopic()->subEvtRsp());

    for(;;){
        string topic, msg;
        recvQ.waitMsg(topic, msg);
        if(topic == dev.getTopic()->subProSet()){
            procProSet(msg);
        } 
        else if(topic == dev.getTopic()->subProGet()){
            procProGet(msg);
        } 
        else if(topic == dev.getTopic()->subCmd()){
            procCmd(msg);
        } 
        else if(topic == dev.getTopic()->subEvtRsp()){
            procEvtRsp(msg);
        } 
        else{
            log_d("unknow topic: %s", topic.c_str());
        }
    }
}