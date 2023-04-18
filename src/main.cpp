#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <stdlib.h>
#include <unistd.h>
#include "queue.h"
//async
#include <future>
//memcpy
#include <string.h>

using namespace std;

#pragma pack(1)
typedef struct 
{
    uint8_t length;
    uint8_t sum;
    uint16_t version;
    uint32_t tarNetCode;
    uint32_t myNetCode;
    uint8_t data[88];
} TransMsg_t;

typedef struct{
    uint8_t type;
    uint8_t subType;
    uint16_t msgID;
    uint8_t data[84];
} AppMsg_t;
#pragma pack()

static uint8_t packAppMsg(uint8_t *tar, const uint8_t *src, uint8_t len)
{
    AppMsg_t *pMsg = (AppMsg_t *)tar;
    pMsg->type = getType();
    pMsg->subType = getSubType();
    pMsg->msgID = getMsgID();
    memcpy(pMsg->data, src, len);

    return 4 + len;
}

#define VERSION 0x0001
#define TRANS_MSG_INIT {0,0,VERSION,{0}};
static uint8_t packTransMsg(uint8_t *src, uint8_t len)
{
    TransMsg_t msg = TRANS_MSG_INIT;

    msg.myNetCode = getMyNetCode();
    msg.tarNetCode = getTarNetCode();
    msg.length = len + 12;
    memcpy(msg.data, src, len);
    msg.sum = checkSum((uint8_t *)&msg, msg.length);

    memcpy(src, &msg, msg.length);
    return msg.length;
}

#define NET_HEAD {0xCC, 0xF0}
#define NET_FOOT 0xDD
static uint8_t packNetMsg(uint8_t *src, uint8_t len)
{
    uint8_t buf[100] = NET_HEAD;

    memcpy(buf + 2, src, len);
    buf[len + 2] = NET_FOOT;
    memcpy(src, buf, len + 3);

    return len + 3;
}

static uint8_t packMsg(uint8_t *tar, const uint8_t *src, uint8_t len)
{
    uint8_t appMsgLen = packAppMsg(tar, src, len);
    uint8_t transMsgLen = packTransMsg(tar, appMsgLen);
    uint8_t netMsgLen = packTransMsg(tar, transMsgLen);

    return netMsgLen;
}

int main()
{
    uint8_t appMsgLen = 10;
    uint8_t appMsg[84];
    uint8_t sendBuf[100];
    packMsg(sendBuf, appMsg, appMsgLen);
}