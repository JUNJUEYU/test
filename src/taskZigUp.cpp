#include "taskZigUp.h"
#include "msgQueue.h"
#include "zigbee.h"
#include "deviceManage.h"
#include "tools.h"
#include "dataBase.h"
#include "db.h"


static void processEvent(DataBase &data)
{
    Db db;

    if(!db.isRegInEvent(data.getPd(), data.getReg(), data.getChan())){
        log_d("reg not in event");
        return;
    }

    uint8_t eventCount = db.getEventCount(data.getPd(), data.getReg(), data.getChan());
    for(int i = 0; i < eventCount; i++){
        string eventId = db.getEventId(data.getPd(), data.getReg(), data.getChan(), i);
        uint8_t objCount = db.getEventObjCount(data.getPd(), eventId);
        for(int j = 0; j < objCount; j ++){
            
        }
    }


    
}


static bool unpackData(ZigMsg_t *msg, DataBase &tar)
{
    DevManager &devManager = DevManager::getInstance();
    string pd = devManager.getDevPd(msg->transLayer.srcNid);
    string sn = devManager.getDevSn(msg->transLayer.srcNid);
    string reg = Trans::toHexString(msg->appLayer.reg);
    string chan = Trans::toHexString(msg->appLayer.chan);

    Db db;
    if(!db.isRegLegal(pd, reg, chan)){
        log_d("zigbee msg reg error");
        return false;
    }
    
    DataBase data(pd, sn, reg, chan);
    uint8_t paramCount = db.getRegParamCount(pd, reg, chan);

    uint8_t offset = 0;
    for(int i = 0; i < paramCount; i++){
        string type = db.getRegParamType(pd, reg, chan, i);
        string param;
        if("uint8" == type){
            param.insert(param.end() + offset, msg->data + offset, msg->data + offset + 1);
            data.addParam(uint8_e, param);
            offset += 1;
        }
        else if("uint16" == type){
            param.insert(param.end() + offset, msg->data + offset, msg->data + offset + 2);
            data.addParam(uint16_e, param);
            offset += 2;
        }
        else if("uint32" == type){
            param.insert(param.end() + offset, msg->data + offset, msg->data + offset + 4);
            data.addParam(uint32_e, param);
            offset += 4;
        }
        else if("int32" == type){
            param.insert(param.end() + offset, msg->data + offset, msg->data + offset + 4);
            data.addParam(int32_e, param);
            offset += 4;
        }
        else if("float32" == type){
            param.insert(param.end() + offset, msg->data + offset, msg->data + offset + 4);
            data.addParam(float32_e, param);
            offset += 4;
        }
        else if("text" == type){
            uint8_t len = msg->data[offset];
            param.insert(param.end() + offset, msg->data + offset + 1, msg->data + offset + 1 + len);
            data.addParam(text_e, param);
            offset += 1 + len;
        }
        else{
            log_d("zigbee msg param type error");
            return false;
        }
    }

    tar = data;
    return true;
}


void *taskZigUp(void *arg)
{
    for(;;){
        int len = 0;
        vector<uint8_t> msg;
        DevMsgRecvQueue &recvQ = DevMsgRecvQueue::getInstance();
        len = recvQ.waitMsg(msg);
        if(0 == len){
            continue;
        }

        ZigMsg_t *pZigMsg = (ZigMsg_t *)msg.data();
        uint16_t appLen = pZigMsg->transLayer.len;
        if(appLen != len - sizeof(ZigMsgNetLayer_t)){
            log_d("zigbee msg len error");
            continue;
        }

        uint8_t sum = Sum::get(6, msg);
        if(sum != pZigMsg->transLayer.sum){
            log_d("zigbee msg sum error");
            continue;
        }

        DevManager &devManager = DevManager::getInstance();
        if(!devManager.isInDevList(pZigMsg->transLayer.srcNid)){
            log_d("zigbee msg srcNid error");
            continue;
        }

        DataBase data;
        if(!unpackData(pZigMsg, data)){
            log_d("zigbee msg unpack error");
            continue;
        }
        
        processEvent(data);

    }
}

/* ----- End of file ----- */
