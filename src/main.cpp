#include "msgQueue.h"
#include "subDevManager.h"
#include "pluginAPI.h"
#include "protocolStack.h"
#include "tools.h"
#include "log.h"
#include "threadComRead.h"
#include <stdlib.h>
#include "test.h"
using namespace std;


int main(int argc, char **argv)
{
    ProtocolStack ps;
    // vector<uint8_t> data = packDataTest();

    // Log::print("data: ", data);

    // Msg *msg = ps.getDeviceMsg(data);
    // Log::print("msg: ", msg->getHexMsg());
    // delete msg;

    ThreadManager &tm = ThreadManager::getInstance();
    tm.addThread(new ComReadThread);
    tm.startAll();
    tm.joinAll();
}