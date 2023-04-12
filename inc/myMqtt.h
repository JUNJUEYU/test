#pragma once
#include "MQTTClient.h"
#include "MQTTAsync.h"
#include "MQTTClientPersistence.h"



class Mqtt{
    public:
        Mqtt(const char *server, const char *clientID);
        ~Mqtt();
        bool connect();
        bool disconnect();
        bool publish(const char *topic, const char *msg);
        bool subscribe(const char *topic);
        bool unsubscribe(const char *topic);
        bool isConnected();
        void setCallback(void (*callback)(char *topic, char *msg));
        void loop();
    private:
        MQTTClient client;
        MQTTClient_connectOptions conn_opts;
        MQTTClient_message pubmsg;
        MQTTClient_deliveryToken token;
        bool isConnected_m;
        void (*callback_m)(char *topic, char *msg);
};


/* ----- End of file ----- */