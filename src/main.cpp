#include "msgQueue.h"
#include "subDevManager.h"
#include "pluginAPI.h"
#include "protocolStack.h"
#include "tools.h"
#include "log.h"
#include "taskComRead.h"
#include <stdlib.h>
#include "test.h"
#include "ts30Event.h"
#include "threadPool.h"
#include <memory>
using namespace std;

static void *task(void *arg)
{
    auto_ptr<Event> event((Event *)arg);
    event->run();
    return NULL;
}

int main(int argc, char **argv)
{
    MessageQueue<string> cmdMsgQ;
    MessageQueue<vector<uint8_t> > devMsgQ;
    SubDeviceManager &devManager = SubDeviceManager::getInstance();
    ThreadManager &tManager = ThreadManager::getInstance();

    EventMaker &eventMaker = TS30Event::getInstance();
    ThreadPool &pool = ThreadPool::getInstance(); 

    Event *event1 = eventMaker.createEvent(NULL,"test1 HAHAH");
    pool.addTask(task, event1);
    pool.addTask(taskComRead, NULL);

    pool.joinAll();
}