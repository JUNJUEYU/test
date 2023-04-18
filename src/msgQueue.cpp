#include "msgQueue.h"
#include "log.h"

NetMsgSendQueue &NetMsgSendQueue::getInstance()
{
    static NetMsgSendQueue instance;
    return instance;
}

NetMsgSendQueue::NetMsgSendQueue() : MessageQueue<NetMsg_t>() {}
NetMsgSendQueue::~NetMsgSendQueue(){};

int NetMsgSendQueue::pushMsg(string topic, string msg)
{
    NetMsg_t netMsg = make_pair(topic, msg);
    push(netMsg);
    return 0;
}

int NetMsgSendQueue::waitMsg(string &topic, string &msg)
{
    NetMsg_t netMsg = pop();
    msg = netMsg.second;
    topic = netMsg.first;
    return 1;
}

NetMsgRecvQueue &NetMsgRecvQueue::getInstance()
{
    static NetMsgRecvQueue instance;
    return instance;
}

NetMsgRecvQueue::NetMsgRecvQueue() : MessageQueue<NetMsg_t>() {}
NetMsgRecvQueue::~NetMsgRecvQueue(){};
int NetMsgRecvQueue::waitMsg(string &topic, string &msg)
{
    NetMsg_t netMsg = pop();
    msg = netMsg.second;
    topic = netMsg.first;
    return 1;
}

int NetMsgRecvQueue::pushMsg(string topic, string msg)
{
    NetMsg_t netMsg = make_pair(topic, msg);
    push(netMsg);
    return 0;
}


DevMsgSendQueue &DevMsgSendQueue::getInstance()
{
    static DevMsgSendQueue instance;
    return instance;
}
DevMsgSendQueue::DevMsgSendQueue() : MessageQueue<vector<uint8_t> >(){};
DevMsgSendQueue::~DevMsgSendQueue(){};
int DevMsgSendQueue::pushMsg(vector<uint8_t> msg)
{
    push(msg);
    return msg.size();
}

int DevMsgSendQueue::waitMsg(vector<uint8_t> &msg)
{
    msg = pop();
    return msg.size();
}

DevMsgRecvQueue &DevMsgRecvQueue::getInstance()
{
    static DevMsgRecvQueue instance;
    return instance;
}

DevMsgRecvQueue::DevMsgRecvQueue() : MessageQueue<vector<uint8_t> >(){};
DevMsgRecvQueue::~DevMsgRecvQueue(){};

int DevMsgRecvQueue::waitMsg(vector<uint8_t> &msg)
{
    msg = pop();
    return msg.size();
}

int DevMsgRecvQueue::pushMsg(vector<uint8_t> msg)
{
    push(msg);
    return msg.size();
}

BlockQueue::BlockQueue(uint32_t timeout) : timeout_m(timeout) 
{
}

int BlockQueue::waitMsg(string &msg, uint32_t timeout = 5)
{
    timeout_m = timeout;
    msg = popTimed(timeout_m);
    return msg.size();
}
  
int BlockQueue::pushMsg(string msg)
{
    push(msg);
    return msg.size();
}


/* ----- End of file ----- */
