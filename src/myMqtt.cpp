#include "myMqtt.h"
#include "log.h"
#include <string.h>
#include "MQTTClient.h"
#include "MQTTAsync.h"
#include "MQTTClientPersistence.h"

static recvCallback_t recvCallback_g = NULL;

static void onConnectSuccess(void *context, MQTTAsync_successData *response)
{
    log_d("onConnectSuccess");
    Mqtt &mqtt = Mqtt::getInstance();
    mqtt.reSubscribe();
}

static void onConnectFailure(void *context, MQTTAsync_failureData *response)
{
    log_d("onConnectFailure");
}

static void onDisconnectSuccess(void *context, MQTTAsync_successData *response)
{
    log_d("onDisconnectSuccess");
}

static void onDisconnectFailure(void *context, MQTTAsync_failureData *response)
{
    log_d("onDisconnectFailure");
}

static void onSubscribeSuccess(void *context, MQTTAsync_successData *response)
{
    log_d("onSubscribeSuccess");
}

static void onSubscribeFailure(void *context, MQTTAsync_failureData *response)
{
    log_d("onSubscribeFailure");
}

static void onUnsubscribeSuccess(void *context, MQTTAsync_successData *response)
{
    log_d("onUnsubscribeSuccess");
}

static void onUnsubscribeFailure(void *context, MQTTAsync_failureData *response)
{
    log_d("onUnsubscribeFailure");
}

static void onConnectionLost(void *context, char *cause)
{
    log_d("onConnectionLost");
    Mqtt &mqtt = Mqtt::getInstance();
    mqtt.connect();

    if (cause != NULL)
    {
        log_d("onConnectionLost: %s", cause);
    }
}

static int onMessageArrived(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{
    log_d("onMessageArrived");
    if (recvCallback_g != NULL)
    {
        string topic(topicName);
        string msg((char *)message->payload);
        recvCallback_g(topic, msg);
    }
    return 1;
}

static void onDeliveryComplete(void *context, MQTTAsync_token token)
{
    log_d("onDeliveryComplete");
}

Mqtt &Mqtt::getInstance()
{
    static Mqtt instance(MQTT_SERVER, MQTT_CLIENTID);
    return instance;
}

Mqtt::Mqtt(const char *server, const char *clientid):client_m(NULL), rc_m(0), connected_m(false)
{
    MQTTAsync_connectOptions connOpts = MQTTAsync_connectOptions_initializer;
    MQTTAsync_disconnectOptions discOpts = MQTTAsync_disconnectOptions_initializer;
    MQTTAsync_responseOptions respOpts = MQTTAsync_responseOptions_initializer;
    MQTTAsync_message pubmsg = MQTTAsync_message_initializer;

    connOpts_m = connOpts;
    discOpts_m = discOpts;
    respOpts_m = respOpts;
    pubmsg_m = pubmsg;
    connected_m = false;

    connOpts_m.keepAliveInterval = 100;
    connOpts_m.cleansession = 1;
    connOpts_m.onSuccess = onConnectSuccess;
    connOpts_m.onFailure = onConnectFailure;
    connOpts_m.username = MQTT_USER;
    connOpts_m.password = MQTT_PASS;
    connOpts_m.context = client_m;
    discOpts_m.onSuccess = onDisconnectSuccess;
    discOpts_m.onFailure = onDisconnectFailure;
    discOpts_m.context = client_m;
    respOpts_m.onSuccess = onSubscribeSuccess;
    respOpts_m.onFailure = onSubscribeFailure;
    respOpts_m.context = client_m;
    pubmsg_m.qos = 1;
    pubmsg_m.retained = 0;
    pubmsg_m.payload = NULL;
    pubmsg_m.payloadlen = 0;
    token_m = 0;

    rc_m = MQTTAsync_create(&client_m, server, clientid, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    rc_m = MQTTAsync_setCallbacks(client_m, NULL, onConnectionLost, onMessageArrived, onDeliveryComplete);
    if(rc_m != MQTTASYNC_SUCCESS)
    {
        log_e("MQTTAsync_setCallbacks failed, rc_m=%d", rc_m);
    }
    log_d("MQTTAsync_setCallbacks success");

    connect();
}

Mqtt::~Mqtt()
{
    MQTTAsync_destroy(&client_m);
}

void Mqtt::connect()
{
    rc_m = MQTTAsync_connect(client_m, &connOpts_m);
    if(rc_m != MQTTASYNC_SUCCESS)
    {
        log_e("MQTTAsync_connect failed, rc_m=%d", rc_m);
    }
    else {
        log_d("MQTTAsync_connect success");
        connected_m = true;
    }
    usleep(500000);
}

void Mqtt::reConnect()
{
    rc_m = MQTTAsync_disconnect(client_m, &discOpts_m);
    rc_m = MQTTAsync_connect(client_m, &connOpts_m);
    if(rc_m != MQTTASYNC_SUCCESS)
    {
        log_e("MQTTAsync_connect failed, rc_m=%d", rc_m);
    }
    else {
        log_d("MQTTAsync_connect success");
        connected_m = true;
    }
}

void Mqtt::pub(const char *topic, const char *msg)
{
    while(!connected_m)
    {
        log_e("MQTT not connected");
        usleep(10000);
    }

    pubmsg_m.payload = (void *)msg;
    pubmsg_m.payloadlen = strlen(msg);
    rc_m = MQTTAsync_sendMessage(client_m, topic, &pubmsg_m, &respOpts_m);
    if (rc_m != MQTTASYNC_SUCCESS)
    {
        log_e("MQTTAsync_sendMessage failed, rc_m=%d", rc_m);
    }
    else{
        log_d("MQTTAsync_sendMessage success");
    }
}

void Mqtt::sub(const char *topic)
{
    while(!connected_m)
    {
        log_e("MQTT not connected");
        usleep(10000);
    }
    
    rc_m = MQTTAsync_subscribe(client_m, topic, 1, &respOpts_m);
    if (rc_m != MQTTASYNC_SUCCESS)
    {
        log_e("MQTTAsync_subscribe failed, rc_m=%d", rc_m);
    }
    else{
        log_d("MQTTAsync_subscribe success");
    }
}

void Mqtt::unsubscribe(const char *topic)
{
    rc_m = MQTTAsync_unsubscribe(client_m, topic, &respOpts_m);
    if (rc_m != MQTTASYNC_SUCCESS)
    {
        log_e("MQTTAsync_unsubscribe failed, rc_m=%d", rc_m);
    }
    else{
        log_d("MQTTAsync_unsubscribe success");
    }
}

void Mqtt::setRecvCallback(recvCallback_t callback)
{
    recvCallback_g = callback;
}

void Mqtt::publish(const string &topic, const string &msg)
{
    pub(topic.c_str(), msg.c_str());
}

void Mqtt::subscribe(const string &topic)
{
    for(vector<string>::iterator it = subTopics_m.begin(); it != subTopics_m.end(); it++)
    {
        if(*it == topic)
        {
            log_d("topic %s already subscribed", topic.c_str());
            return;
        }
    }
    subTopics_m.push_back(topic);
    sub(topic.c_str());
}

void Mqtt::reSubscribe()
{
    for(vector<string>::iterator it = subTopics_m.begin(); it != subTopics_m.end(); it++)
    {
        sub(it->c_str());
    }
}

/* ----- End of file ----- */
