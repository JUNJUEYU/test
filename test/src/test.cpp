#include "msgQueue.h"
#include "tools.h"
#include "log.h"
#include <stdlib.h>
#include "cJSON.h"
#include "zigbee.h"
#include "tableCommand.h"
#include "tableProperty.h"
#include "tableEvent.h"

#define FILE_PATH "./test.json"

void getZigbeeId(){
    vector<uint8_t> msg1;
    msg1.push_back(0xAB);
    msg1.push_back(0xD1);
    msg1.push_back(0xBA);
    DevMsgSendQueue &devMsgSendQueue = DevMsgSendQueue::getInstance();
    devMsgSendQueue.pushMsg(msg1);
}

string testJson()
{
    // char * file = NULL;
    // FILE *fp = fopen(FILE_PATH, "r");
    // if (NULL == fp)
    // {
    //     log_i("open file error");
    //     return "";
    // }
    // fseek(fp, 0, SEEK_END);
    // long len = ftell(fp);
    // fseek(fp, 0, SEEK_SET);
    // file = (char *)malloc(len + 1);
    // fread(file, 1, len, fp);
    // file[len] = '\0';
    // fclose(fp);
    // cJSON *root = cJSON_Parse(file);
    // if (NULL == root)
    // {
    //     log_i("json parse error");
    //     return "";
    // }
    // int size = cJSON_GetArraySize(root);
    // log_i("size = %d", size);
    // for (size_t i = 0; i < size; i++)
    // {
    //     uint16_t reg;
    //     cJSON *item = cJSON_GetArrayItem(root, i);
    //     if (NULL != item)
    //     {
    //         reg = Trans::toHexInt(string(item->string));
    //         ZigReg zigReg(reg);
    //         log_i("reg = %04x", reg);
    //         int count = cJSON_GetArraySize(item);
    //         log_i("count = %d", count);
    //         for (size_t j = 0; j < count; j++)
    //         {
    //             cJSON *item1 = cJSON_GetArrayItem(item, j);
    //             if (NULL != item1)
    //             {
    //                 cJSON *num = cJSON_GetObjectItem(item1, "num");
    //                 if (NULL != num)
    //                 {
    //                     log_i("num = %d", num->valueint);
    //                 }
    //                 cJSON *type = cJSON_GetObjectItem(item1, "type");
    //                 if (NULL != type)
    //                 {
    //                     log_i("type = %s", type->valuestring);
    //                 }

    //                 ZigData zigData(num->valueint, type->valuestring);
    //                 zigReg.addData(zigData);
    //             }
    //         }
    //         for(int i = 0; i < zigReg.getDataSize(); i++){
    //             ZigData zigData = zigReg.getData(i);
    //             log_i("num = %d, type = %s", zigData.num_m, zigData.type_m.c_str());
    //         }
    //         ZigRegTable::getInstance().addReg(zigReg);
    //     }
    // }    
    // cJSON_Delete(root);
    // free(file);
    // return "";
}

void testTable()
{
    CommandTable &commandTable = CommandTable::getInstance();
    Command cmd;
    bool ret = commandTable.getCommand(0x300101, cmd);
    if(ret == false){
        log_e("get command error");
    }
    log_i("cmd name = %s", cmd.name_m.c_str());
    log_i("cmd regAddr = %x", cmd.regAddr_m);
    for (int i = 0; i < cmd.param_m.size(); i++)
    {
        log_i("cmd param name = %s", cmd.param_m[i].name.c_str());
        log_i("cmd param num = %d", cmd.param_m[i].num);
        log_i("cmd param type = %s", cmd.param_m[i].type.c_str());
    }

    PropertyTable &propertyTable = PropertyTable::getInstance();
    Property prop;
    ret = propertyTable.getProperty(0x100101, prop);
    if(ret == false){
        log_e("get property error");
    }

    log_i("prop name = %s", prop.name_m.c_str());
    log_i("prop regAddr = %06x", prop.regAddr_m);
    log_i("prop rw = %s", prop.rw_m.c_str());
    for(int i = 0; i < prop.param_m.size(); i++){
        log_i("prop param name = %s", prop.param_m[i].name.c_str());
        log_i("prop param num = %d", prop.param_m[i].num);
        log_i("prop param type = %s", prop.param_m[i].type.c_str());
 
    }


    EventTable &eventTable = EventTable::getInstance();
    Event event;
    ret = eventTable.getEvent(0x2001, event);
    if(ret == false){
        log_e("get event error");
    }
    log_i("event name = %s", event.name_m.c_str());
    log_i("event regAddr = %04x", event.regAddr_m);
    log_i("event type = %s", event.type_m.c_str());
    log_i("event object = %s", event.object_m.c_str());
    for(int i = 0; i < event.param_m.size(); i++){
        log_i("event param name = %s", event.param_m[i].name.c_str());
        log_i("event param num = %d", event.param_m[i].num);
        log_i("event param type = %s", event.param_m[i].type.c_str());

    }
}



/* ----- End of file ----- */
