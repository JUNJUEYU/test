#pragma once
#include <pthread.h>
#include <vector>
#include <deque>
#include <stdint.h>

class ThreadPool
{
public:
    static ThreadPool &getInstance();
    void addTask(void *(*func)(void *), void *arg);
    void stopAll();
    void joinAll();

private:
    ThreadPool(int threadNum);
    ~ThreadPool();
    ThreadPool(const ThreadPool &);
    ThreadPool &operator=(const ThreadPool &);
    static void *threadFunc(void *arg);
    void create();

private:
    int threadNum_m;
    pthread_t *pThreadIds_m;
    std::deque<pthread_t> threadList_m;
    std::deque<void *(*)(void *)> taskList_m;
    std::deque<void *> argList_m;
    pthread_mutex_t mutex_m;
    pthread_cond_t cond_m;
    bool isExit_m;
};

/* ----- End of file ----- */
