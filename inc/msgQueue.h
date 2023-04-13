#pragma once
#include <queue>
#include <pthread.h>
#include <string>
#include <vector>
#include <stdint.h>
using namespace std;

//泛型阻塞消息队列
template<typename T>
class MessageQueue {
public:
    MessageQueue() {
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&cond, NULL);
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

class NetMsgSendQueue : public MessageQueue<string> {
public:
    static NetMsgSendQueue &getInstance();
private:
    NetMsgSendQueue();
    ~NetMsgSendQueue();
    NetMsgSendQueue(const NetMsgSendQueue &other){};
    NetMsgSendQueue &operator=(const NetMsgSendQueue &other){};
};

class DevMsgSendQueue : public MessageQueue<vector<uint8_t> > {
public:
    static DevMsgSendQueue &getInstance();
private:
    DevMsgSendQueue() ;
    ~DevMsgSendQueue();
    DevMsgSendQueue(const DevMsgSendQueue &other){};
    DevMsgSendQueue &operator=(const DevMsgSendQueue &other){};
};



/* ----- End of file ----- */
