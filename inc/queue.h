#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class Queue
{
public:
    Queue() : m_bStop(false) {}
    ~Queue() {}

    void push(const T& t)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(t);
        m_cond.notify_one();
    }

    T pop(){
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_queue.empty() && !m_bStop)
        {
            m_cond.wait(lock);
        }
        if (m_bStop)
        {
            throw std::runtime_error("queue is stopped");
        }
        T t = m_queue.front();
        m_queue.pop();
        return t;
    }

    void stop()
    {
        m_bStop = true;
        m_cond.notify_all();
    }

    bool empty()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    size_t size()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    bool m_bStop;
};




