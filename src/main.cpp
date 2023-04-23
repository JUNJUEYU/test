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


    ZigRegTable &zigRegTable = ZigRegTable::getInstance();
    pool.addTask(taskComRead, NULL);
    pool.addTask(taskComSend, NULL);
    pool.addTask(taskZigUp, NULL);
    pool.addTask(taskMqttRecv, NULL);
    
    pool.joinAll();
}