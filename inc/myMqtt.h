#pragma once
#include "MQTTClient.h"
#include "MQTTAsync.h"
#include "MQTTClientPersistence.h"
#include <string>
#include <vector>
using namespace std;

#define MQTT_SERVER "mqtt://192.168.0.2:1883"
#define MQTT_USER "admin"
#define MQTT_PASS "123456"
#define MQTT_CLIENTID "iot-gw-001"
#define MQTT_TOPIC "/1/2/3"

typedef void *(*recvCallback_t)(string &topic, string &msg);

class Mqtt{
    public:
        static Mqtt &getInstance();
        void connect();
        void reConnect();
        void unsubscribe(const char *topic);
        void setRecvCallback(recvCallback_t callback);
        void subscribe(const string &topic);
        void publish(const string &topic, const string &msg);
        void reSubscribe();

    private:
        void pub(const char *topic, const char *msg);
        void sub(const char *topic);

    private:
        Mqtt(const char *server, const char *clientid);
        ~Mqtt();
        Mqtt &operator=(const Mqtt &mqtt);
        Mqtt(const Mqtt &mqtt);
        MQTTAsync client_m;
        MQTTAsync_connectOptions connOpts_m ;
        MQTTAsync_disconnectOptions discOpts_m;
        MQTTAsync_responseOptions respOpts_m ;
        MQTTAsync_message pubmsg_m;
        MQTTAsync_token token_m;
        int rc_m;
        bool connected_m;
        vector<string> subTopics_m;
};


/* ----- End of file ----- */