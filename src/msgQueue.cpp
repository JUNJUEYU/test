#include "msgQueue.h"

NetMsgSendQueue &NetMsgSendQueue::getInstance()
{
    static NetMsgSendQueue instance;
    return instance;
}

NetMsgSendQueue::NetMsgSendQueue() : MessageQueue<string>() {}
NetMsgSendQueue::~NetMsgSendQueue(){};

DevMsgSendQueue &DevMsgSendQueue::getInstance()
{
    static DevMsgSendQueue instance;
    return instance;
}
DevMsgSendQueue::DevMsgSendQueue() : MessageQueue<vector<uint8_t> >(){};
DevMsgSendQueue::~DevMsgSendQueue(){};

/* ----- End of file ----- */
