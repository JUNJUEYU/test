#include "taskNet.h"
#include "log.h"
#include "myMqtt.h"
#include "msgQueue.h"

static Mqtt *pMqtt_g = NULL;


void netRecvCallback(char *topic, char *msg)
{
    log_d("netRecvCallback");
    log_d("recv topic = %s",topic);
    log_d("recv msg = %s", msg);
    NetMsgRecvQueue::getInstance().pushMsg(topic, msg);
}

static void netInit(void)
{
    if(NULL != pMqtt_g){
        return;
    }
    
    pMqtt_g = new Mqtt(MQTT_SERVER, MQTT_CLIENTID);
    if(NULL == pMqtt_g){
        log_d("new Mqtt failed");
        return;
    }

    if(false == pMqtt_g->connect()){
        log_d("connect Mqtt failed");
        return;
    }

    if(false == pMqtt_g->subscribe(MQTT_TOPIC)){
        log_d("subscribe Mqtt failed");
        return;
    }

    log_d("new Mqtt success");
    pMqtt_g->setCallback(netRecvCallback);

}

void *taskNetSend(void *arg)
{
    netInit();  

    for(;;){
        if(false == pMqtt_g->isConnected()){
            pMqtt_g->connect();
        }

        string msg, topic;
        if(0 == NetMsgSendQueue::getInstance().waitMsg(topic, msg)){
            log_d("send msg length = 0");
            continue;
        }

        pMqtt_g->publish(topic.c_str(), msg.c_str());
    }
}


void *taskNetRead(void *arg)
{
    netInit();  

    for(;;){
        pMqtt_g->loop();
    }
}

/* ----- End of file ----- */
