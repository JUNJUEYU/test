#pragma once
#include <pthread.h>
#include <vector>
#include "log.h"

using namespace std;
class Thread
{
public:
    Thread(){};
    virtual ~Thread(){};
    void start()
    {
        pthread_create(&threadId_m, NULL, threadFunc, this);
    };
    void join()
    {
        pthread_join(threadId_m, NULL);
        Log::print("Thread::join");
    };
    virtual void run() = 0;

private:
    pthread_t threadId_m;
    static void *threadFunc(void *arg)
    {
        Thread *thread = static_cast<Thread *>(arg);
        thread->run();
        return NULL;
    };
};

class ThreadManager
{  
public:
    static ThreadManager &getInstance()
    {
        static ThreadManager instance;
        return instance;
    };

    void addThread(Thread *thread)
    {
        threadList_m.push_back(thread);
    };

    void startAll()
    {
        vector<Thread *>::iterator it;
        for (it = threadList_m.begin(); it != threadList_m.end(); it++)
        {
            (*it)->start();
        }
    };

    void joinAll()
    {
        vector<Thread *>::iterator it;
        for (it = threadList_m.begin(); it != threadList_m.end(); it++)
        {
            (*it)->join();
            delete *it;
        }
    };
    
private:
    ThreadManager(){};
    virtual ~ThreadManager(){
        vector<Thread *>::iterator it;
        for (it = threadList_m.begin(); it != threadList_m.end(); it++)
        {
            if(*it)
                delete *it;
        }
    };
    ThreadManager &operator=(const ThreadManager &){};
    ThreadManager(const ThreadManager &){};

    std::vector<Thread *> threadList_m;
};

/* ----- End of file ----- */
