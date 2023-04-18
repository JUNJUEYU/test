#include <stdlib.h>
#include "tools.h"
#include "log.h"
#include "test.h"
#include "threadPool.h"
#include "taskNet.h"
#include "taskCom.h"
#include "taskDevMsgProcess.h"
#include "msgQueue.h"
#include <vector>
#include "subDevManager.h"
#include "db.h"
using namespace std;



int main(int argc, char **argv)
{
    Log::init();
    ThreadPool &pool = ThreadPool::getInstance(); 
    DevMsgRecvQueue &devMsgRecvQueue = DevMsgRecvQueue::getInstance();

    vector<uint8_t> msg;
    Db db;
    log_e("main start dbver = %s", sqlite3_libversion());  

    log_d("dev count = %d", db.getDevCount()); 
    if(db.isLegalDev("ts30-20230418-01")){
        log_d("dev name = %s",db.getDevName("ts30-20230418-01").c_str());
        log_d("legal dev");
    }
    else{
        log_d("illegal dev");
    }

    if(db.isLegalDev("ts30-20230418-06")){
        log_d("legal dev 06");
    }
    else{
        log_d("illegal dev 06");
    }

    

    db.delRegDev(1234);
    

    pool.addTask(taskComRead, NULL);
    pool.addTask(taskComSend, NULL);
    // pool.addTask(taskNetRead, NULL);
    // pool.addTask(taskNetSend, NULL);
    pool.addTask(taskDevMsgProcess, NULL);

    msg = testNetAccStatic();
    devMsgRecvQueue.pushMsg(msg);
    sleep(1);   
    msg = testSigUp();
    devMsgRecvQueue.pushMsg(msg);

    pool.joinAll();
}