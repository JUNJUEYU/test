#pragma once
#include <queue>
#include <pthread.h>
#include <string>
#include <vector>
#include <stdint.h>
#include <utility>
#include <time.h>
#include <errno.h>
#include "log.h"

using namespace std;

//泛型阻塞消息队列
template<typename T>
class MessageQueue {
protected:
    MessageQueue() {
        pthread_condattr_t condattr;
        pthread_mutex_init(&mutex, NULL);
        pthread_condattr_setclock(&condattr, CLOCK_MONOTONIC);
        pthread_cond_init(&cond, &condattr);
    }

    ~MessageQueue() {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }

    void push(T value) {
        pthread_mutex_lock(&mutex);
        queue.push(value);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    T pop() {
        pthread_mutex_lock(&mutex);
        while (queue.empty()) {
            pthread_cond_wait(&cond, &mutex);
        }
        T value = queue.front();  
        queue.pop();
        pthread_mutex_unlock(&mutex);
        return value;
    }

    T popTimed(uint32_t timeout) {
        pthread_mutex_lock(&mutex);
        while (queue.empty()) {
            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC, &ts);
            ts.tv_sec += timeout;
            if (pthread_cond_timedwait(&cond, &mutex, &ts) == ETIMEDOUT) {
                log_d("pthread_cond_timedwait return ETIMEDOUT");
                pthread_mutex_unlock(&mutex);
                return T();
            }
        }
        T value = queue.front();
        queue.pop();
        pthread_mutex_unlock(&mutex);
        return value;
    }

    int size() {
        pthread_mutex_lock(&mutex);
        int size = queue.size();
        pthread_mutex_unlock(&mutex);
        return size;
    }

private:
    std::queue<T> queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};


class BlockQueue : public MessageQueue<string> {
public:
    BlockQueue(uint32_t timeout = 0);
    int waitMsg(string &msg, uint32_t timeout);
    int pushMsg(string msg);
private:
    uint32_t timeout_m;
};


typedef pair<string, string> NetMsg_t;
class NetMsgSendQueue : public MessageQueue<NetMsg_t> {
public:
    static NetMsgSendQueue &getInstance();
    int waitMsg(string &topic, string &msg);
    int pushMsg(string topic, string msg);
private:
    NetMsgSendQueue();
    ~NetMsgSendQueue();
    NetMsgSendQueue(const NetMsgSendQueue &other){};
    NetMsgSendQueue &operator=(const NetMsgSendQueue &other){};
};

class NetMsgRecvQueue : public MessageQueue<NetMsg_t> {
public:
    static NetMsgRecvQueue &getInstance();
    int waitMsg(string &topic, string &msg);
    int pushMsg(string topic, string msg);
private:
    NetMsgRecvQueue();
    ~NetMsgRecvQueue();
    NetMsgRecvQueue(const NetMsgRecvQueue &other){};
    NetMsgRecvQueue &operator=(const NetMsgRecvQueue &other){};
};

class DevMsgSendQueue : public MessageQueue<vector<uint8_t> > {
public:
    static DevMsgSendQueue &getInstance();
    int waitMsg(vector<uint8_t> &msg);
    int pushMsg(vector<uint8_t> msg);
private:
    DevMsgSendQueue() ;
    ~DevMsgSendQueue();
    DevMsgSendQueue(const DevMsgSendQueue &other){};
    DevMsgSendQueue &operator=(const DevMsgSendQueue &other){};
};

class DevMsgRecvQueue : public MessageQueue<vector<uint8_t> > {
public:
    static DevMsgRecvQueue &getInstance();
    int waitMsg(vector<uint8_t> &msg);
    int pushMsg(vector<uint8_t> msg);
private:
    DevMsgRecvQueue() ;
    ~DevMsgRecvQueue();
    DevMsgRecvQueue(const DevMsgRecvQueue &other){};
    DevMsgRecvQueue &operator=(const DevMsgRecvQueue &other){};
};


/* ----- End of file ----- */
