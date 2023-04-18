#include "myMqtt.h"
#include "log.h"
#include <string.h>
#include "MQTTClient.h"
#include "MQTTAsync.h"
#include "MQTTClientPersistence.h"

Mqtt::Mqtt(const char *server, const char *clientID)
{
    MQTTClient_create(&client, server, clientID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = MQTT_USER;
    conn_opts.password = MQTT_PASS;
    isConnected_m = false;
}

Mqtt::~Mqtt()
{
    MQTTClient_destroy(&client);
}

bool Mqtt::connect()
{
    int rc = MQTTClient_connect(client, &conn_opts);
    if (rc != MQTTCLIENT_SUCCESS)
    {
        isConnected_m = false;
        return false;
    }
    else
    {
        isConnected_m = true;
        return true;
    }
}

bool Mqtt::disconnect()
{
    int rc = MQTTClient_disconnect(client, 10000);
    if (rc != MQTTCLIENT_SUCCESS)
    {
        isConnected_m = false;
        return false;
    }
    else
    {
        isConnected_m = false;
        return true;
    }
}

bool Mqtt::publish(const char *topic, const char *msg)
{
    if (!isConnected_m)
    {
        return false;
    }
    pubmsg.payload = (void *)msg;
    pubmsg.payloadlen = strlen(msg);
    pubmsg.qos = 1;
    pubmsg.retained = 0;
    int rc = MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    if (rc != MQTTCLIENT_SUCCESS)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Mqtt::subscribe(const char *topic)
{
    if (!isConnected_m)
    {
        return false;
    }
    int rc = MQTTClient_subscribe(client, topic, 1);
    if (rc != MQTTCLIENT_SUCCESS)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Mqtt::unsubscribe(const char *topic)
{
    if (!isConnected_m)
    {
        return false;
    }
    int rc = MQTTClient_unsubscribe(client, topic);
    if (rc != MQTTCLIENT_SUCCESS)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Mqtt::isConnected()
{
    return isConnected_m;
}

void Mqtt::setCallback(void (*callback)(char *topic, char *msg))
{
    callback_m = callback;
    MQTTClient_setCallbacks(client, NULL, NULL,(MQTTClient_messageArrived *)callback_m, NULL);
}

void Mqtt::loop()
{
    MQTTClient_yield();
}





/* ----- End of file ----- */
