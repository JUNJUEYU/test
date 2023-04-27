#include <stdlib.h>
#include "tools.h"
#include "log.h"
#include "test.h"
#include "threadPool.h"
#include "taskNet.h"
#include "taskCom.h"
#include "task.h"
#include "msgQueue.h"
#include <vector>
#include "db.h"
#include "cJSON.h"
#include "zigbee.h"
#include "myMqtt.h"
#include "taskZigUp.h"
#include "taskMqttRecv.h"
#include "descTblRegister.h"

using namespace std;



void test(void)
{
    const char *json = "{\"aa\":\"1\",\"bb\":2}";
    cJSON *root = cJSON_Parse(json);
    if(root == NULL){
        log_e("json parse error");
        return;
    }
    int size = cJSON_GetArraySize(root);
    for(int i = 0 ;i < size; i++){
        cJSON *item = cJSON_GetArrayItem(root, i);
        if(item == NULL){
            log_e("json get item error");
            return;
        }
        log_d("item key = %s", item->string);
        if (item->type == cJSON_String)
        {
            log_d("item value = %s", item->valuestring);
        }
        else if (item->type == cJSON_Number)
        {
            log_d("item value = %d", item->valueint);
        }
        else if (item->type == cJSON_Array)
        {
            /* code */
        }
        else if (item->type == cJSON_Object)
        {
            /* code */
        }
        
    }
}



int main(int argc, char **argv)
{
    Log::init();
    ThreadPool &pool = ThreadPool::getInstance(); 
    DevMsgRecvQueue &devMsgRecvQueue = DevMsgRecvQueue::getInstance();
    vector<uint8_t> msg;
    log_e("main start dbver = %s", sqlite3_libversion());  
    RegisterDescTbl tbl("./register_description_table.json");
    log_i("getRegDescSize = %d", tbl.getRegDescSize());
    RegisterDesc regDesc;
    tbl.getRegDesc(0x2001, regDesc);
    log_i("regAddr = %04x", regDesc.regAddr_m);
    log_i("rw = %s", regDesc.rw_m.c_str());
    for(int i = 0; i < regDesc.proList_m.size(); i++){
        log_i("name = %s", regDesc.proList_m[i].name_m.c_str());
        log_i("offest = %d", regDesc.proList_m[i].offest_m);
        log_i("dataType = %s", regDesc.proList_m[i].dataType_m.c_str());
    }

    Db db;

    


    // ZigRegTable &zigRegTable = ZigRegTable::getInstance();
    // pool.addTask(taskComRead, NULL);
    // pool.addTask(taskComSend, NULL);
    // pool.addTask(taskZigUp, NULL);
    // pool.addTask(taskMqttRecv, NULL);
    
    pool.joinAll();
}