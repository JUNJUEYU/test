#pragma once
#include <stdint.h>
#include <vector>
#include <string>
#include "msgQueue.h"
#include "myMqtt.h"


class MqttMsg{
public:
    virtual string getTopic() = 0;
    virtual string getMsg() = 0;
};

class MqttMsgDataUpload : public MqttMsg{
public:
    MqttMsgDataUpload(const vector<uint8_t> &data) ;
    ~MqttMsgDataUpload(){};
    string getTopic(){
        return topic_m;
    };
    string getMsg(){
        return msg_m;
    };
private:
    string topic_m;
    string msg_m;
};

class MqttMsgFaultUpload : public MqttMsg{
public:
    MqttMsgFaultUpload(const vector<uint8_t> &data) ;
    ~MqttMsgFaultUpload(){};
    string getTopic(){
        return topic_m;
    };
    string getMsg(){
        return msg_m;
    };
private:
    string topic_m;
    string msg_m;
};




/* ----- End of file ----- */

