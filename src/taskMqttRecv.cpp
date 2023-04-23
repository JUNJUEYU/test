#include "taskMqttRecv.h"
#include "log.h"
#include "myMqtt.h"
#include "msgQueue.h"


void *recvfunc(string &topic, string &payload)
{
    NetMsgRecvQueue &recvQ = NetMsgRecvQueue::getInstance();
    recvQ.pushMsg(topic, payload);
    return NULL;
}

void *taskMqttRecv(void *arg)
{
    Mqtt &mqtt = Mqtt::getInstance();
    mqtt.setRecvCallback(recvfunc);
    mqtt.subscribe("/cloud/gw/1/property/set");
    mqtt.subscribe("/cloud/#");
    NetMsgRecvQueue &recvQ = NetMsgRecvQueue::getInstance();

    for(;;){
        string topic, msg;
        recvQ.waitMsg(topic, msg);
        log_i("recvfunc topic = %s, payload = %s", topic.c_str(), msg.c_str());
        mqtt.publish("/gw/1", "fuck you");
    }
}