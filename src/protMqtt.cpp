#include "protMqtt.h"
#include "json.h"

bool MqttProtProSet::parse(const string &msg)
{
    cJSON *root = Json(msg).root();
    if(root == NULL){
        return false;
    }

    cJSON *msgId = cJSON_GetObjectItem(root, "msg_id");
    if(msgId == NULL){
        return false;
    }
    uuid_m = msgId->valuestring;

    cJSON *tarPd = cJSON_GetObjectItem(root, "target_pd");
    if(tarPd == NULL){
        return false;
    }
    tarPd_m = tarPd->valuestring;

    cJSON *tarSn = cJSON_GetObjectItem(root, "target_sn");
    if(tarSn == NULL){
        return false;
    }
    tarSn_m = tarSn->valuestring;

    cJSON *pro = cJSON_GetObjectItem(root, "property");
    if(pro == NULL){
        return false;
    }

    cJSON *proName = cJSON_GetObjectItem(pro, "name");



    return true;
}