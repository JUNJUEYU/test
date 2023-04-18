#pragma once
#include <stdint.h>
#include <vector>
#include <string>
using namespace std;

class MqttProt{
    public:
        virtual ~MqttProt(){};

};


class MqttProtStack{
public:
    typedef enum{
        ACK,
    }MqttMsgType_t;
public:
        MqttProtStack(const string &topic, const string &data):topic_m(topic), data_m(data){};
        ~MqttProtStack(){};
        string getMsgType();
        string getSn();
        string getUuid();
        MqttMsgType_t msgType();
    

protected:
        string topic_m;
        string data_m;
};


/* ----- End of file ----- */
