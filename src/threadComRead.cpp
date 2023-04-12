#include "threadComRead.h"
#include "log.h"
#include <sys/epoll.h>
#include <unistd.h>

ComReadThread::ComReadThread()
{
}

ComReadThread::~ComReadThread()
{
}

void ComReadThread::run()
{
    Log::print("ComReadThread::run");
    int epollFd = epoll_create(1);
    if (epollFd == -1)
    {
        Log::print("epoll_create error");
    }
    else
    {
        Log::print("epoll_create success");
    }

    int count = 0;
    while(1){
        sleep(1);
        Log::print("ComReadThread::run", count++);
        break;
    }
}

/* ----- End of file ----- */
