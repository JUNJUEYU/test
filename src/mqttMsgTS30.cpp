#include "mqttMsgTS30.h"
#include "log.h"

MqttMsgDataUpload::MqttMsgDataUpload(const vector<uint8_t> &data)
{
    topic_m = "data/upload";
    msg_m = "data";
}

MqttMsgFaultUpload::MqttMsgFaultUpload(const vector<uint8_t> &data)
{
    topic_m = "fault/upload";
    msg_m = "fault";
}


/* ----- End of file ----- */
