#include "threadPool.h"
#include "log.h"


ThreadPool::ThreadPool(int threadNum)
{
    threadNum_m = threadNum;
    pThreadIds_m = new pthread_t[threadNum_m];
    isExit_m = false;
    pthread_mutex_init(&mutex_m, NULL);
    pthread_cond_init(&cond_m, NULL);
    create();
}

ThreadPool::~ThreadPool()
{
    if (pThreadIds_m != NULL)
    {
        delete[] pThreadIds_m;
    }
    pthread_mutex_destroy(&mutex_m);
    pthread_cond_destroy(&cond_m);
}

void ThreadPool::addTask(void *(*func)(void *), void *arg)
{
    pthread_mutex_lock(&mutex_m);
    taskList_m.push_back(func);
    argList_m.push_back(arg);
    pthread_cond_signal(&cond_m);
    pthread_mutex_unlock(&mutex_m);
}

void ThreadPool::stopAll()
{
    if (isExit_m)
    {
        return;
    }
    isExit_m = true;
    pthread_cond_broadcast(&cond_m);
    for (int i = 0; i < threadNum_m; i++)
    {
        pthread_join(pThreadIds_m[i], NULL);
    }
}

void ThreadPool::create()
{
    for (int i = 0; i < threadNum_m; i++)
    {
        int ret = pthread_create(&pThreadIds_m[i], NULL, threadFunc, this);
        threadList_m.push_back(pThreadIds_m[i]);
    }
}

void *ThreadPool::threadFunc(void *arg)
{
    ThreadPool *pool = static_cast<ThreadPool *>(arg);
    while (true)
    {
        pthread_mutex_lock(&pool->mutex_m);
        while (pool->taskList_m.empty() && !pool->isExit_m)
        {
            pthread_cond_wait(&pool->cond_m, &pool->mutex_m);
        }
        if (pool->isExit_m)
        {
            pthread_mutex_unlock(&pool->mutex_m);
            pthread_exit(NULL);
        }
        void *(*func)(void *) = pool->taskList_m.front();
        void *arg = pool->argList_m.front();
        pool->taskList_m.pop_front();
        pool->argList_m.pop_front();
        pthread_mutex_unlock(&pool->mutex_m);
        func(arg);
    }
    return NULL;
}

void ThreadPool::joinAll()
{
    std::deque<pthread_t>::iterator it;
    for (it = threadList_m.begin(); it != threadList_m.end(); it++)
    {
        pthread_join(*it, NULL);
    }
}


ThreadPool &ThreadPool::getInstance()
{
    static ThreadPool instance(15);
    return instance;
}


/* ----- End of file ----- */
