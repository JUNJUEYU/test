#include "tableEvent.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "tools.h"

#define FILE_PATH "./command.json"

EventTable::EventTable()
{
    char * file = NULL;
    FILE *fp = fopen(FILE_PATH, "r");
    if (NULL == fp)
    {
        log_i("open file error");
        return ;
    }
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    file = (char *)malloc(len + 1);
    fread(file, 1, len, fp);
    file[len] = '\0';
    fclose(fp);
    cJSON *root = cJSON_Parse(file);
    if (NULL == root)
    {
        log_i("json parse error");
        return ;
    } 

    cJSON *event = cJSON_GetObjectItem(root, "event");
    int size = cJSON_GetArraySize(event);
    for(int i = 0; i < size; i++){
        cJSON *item = cJSON_GetArrayItem(event, i);
        if(NULL == item){
            continue;
        }

        Event evt;
        cJSON *reg = cJSON_GetObjectItem(item, "reg");
        if(NULL == reg){
            continue;
        }
        evt.regAddr_m = Trans::toHexInt(reg->valuestring);

        cJSON *name = cJSON_GetObjectItem(item, "name");
        if(NULL == name){
            continue;
        }
        evt.name_m = name->valuestring;

        cJSON *type = cJSON_GetObjectItem(item, "type");
        if(NULL == type){
            continue;
        }
        evt.type_m = type->valuestring;

        cJSON *object = cJSON_GetObjectItem(item, "object");
        if(NULL == object){
            continue;
        }
        evt.object_m = object->valuestring;

        cJSON *param = cJSON_GetObjectItem(item, "param");
        if(NULL == param){
            continue;
        }

        int paramSize = cJSON_GetArraySize(param);
        for(int j = 0; j < paramSize; j++){
            ParamBase paramBase;
            cJSON *paramItem = cJSON_GetArrayItem(param, j);
            if(NULL == paramItem){
                continue;
            }

            cJSON *paramName = cJSON_GetObjectItem(paramItem, "name");
            if(NULL == paramName){
                continue;
            }
            paramBase.name = paramName->valuestring;

            cJSON *paramValue = cJSON_GetObjectItem(paramItem, "num");
            if(NULL == paramValue){
                continue;
            }
            paramBase.num = paramValue->valueint;

            cJSON *paramType = cJSON_GetObjectItem(paramItem, "type");
            if(NULL == paramType){
                continue;
            }
            paramBase.type = paramType->valuestring;
            evt.param_m.push_back(paramBase);
        }
        evtList_m.push_back(evt);
    } 
    cJSON_Delete(root);
    free(file);
}

bool EventTable::getEvent(uint32_t regAddr, Event &evt)
{
    for(vector<Event>::iterator it = evtList_m.begin(); it != evtList_m.end(); it++){
        if(it->regAddr_m == regAddr){
            evt = *it;
            return true;
        }
    }
    return false;
}

bool EventTable::getEvent(string name, Event &evt)
{
    for(vector<Event>::iterator it = evtList_m.begin(); it != evtList_m.end(); it++){
        if(it->name_m == name){
            evt = *it;
            return true;
        }
    }
    return false;
}

EventTable& EventTable::getInstance()
{
    static EventTable instance;
    return instance;
}