#pragma once
#include <queue>
#include <pthread.h>

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

/* ----- End of file ----- */
