#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP
#include <sys/time.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <pthread.h> 
#include <unistd.h>
#include <dirent.h>

#include <thread>
#include <future>
#include "MyEasyLog.hpp"
#include <list>

#include <atomic>

template <typename T_task>
class SyncQueue {
private:
    bool m_flagStop;
    std::mutex m_mutex;//用于存取互斥的
    std::condition_variable m_condNonEmpty;
    std::condition_variable m_condNonFull;
    int32_t m_lengthQueueMax;
    std::list<T_task> m_queue;
private:
    bool IsFull(void) const {
        bool isFull = (m_queue.size() >= m_lengthQueueMax);
        if (isFull)
            LOG_INFO("queue is full, please wait...\n");
        return isFull;
    }
    bool IsEmpty(void) const {
        bool isEmpty = m_queue.empty();
        if (isEmpty)
            LOG_INFO("queue is empty, task thread wati, tid:%ld\n", std::this_thread::get_id());
        return isEmpty;
    }
    template<typename F>
    void Add(F&& x) {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_condNonFull.wait(locker, [this] {return m_flagStop || (!IsFull());});//当lambda返回ture时，即未满时，继续执行，否则等待；等待方；
        if (m_flagStop)
            return;
        m_queue.push_back(std::forward<F>(x));
        m_condNonEmpty.notify_one();//add notify take
    }
public:
    SyncQueue(int32_t lengthMax) : m_lengthQueueMax(lengthMax), m_flagStop(false) {};

    void Put(const T_task& x) {
        Add(x);
    }
    
    void Put(T_task&& x) {
        Add(std::forward<T_task>(x));
    }
    
    void Take(std::list<T_task>& list) {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_condNonEmpty.wait(locker, [this] {return m_flagStop || (!IsEmpty());});
        if (m_flagStop)
            return;
        list = std::move(m_queue);
        m_condNonFull.notify_one();//take notify add
    }
    
    void Take(T_task& t) {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_condNonEmpty.wait(locker, [this] {return m_flagStop || (!IsEmpty());});
        if (m_flagStop)
            return;
        t = m_queue.front();
        m_queue.pop_front();
        m_condNonFull.notify_one();
    }

    void Stop() {
        {
            std::lock_guard<std::mutex> locker(m_mutex);
            m_flagStop = true;
        }
        m_condNonEmpty.notify_all();
        m_condNonFull.notify_all();
    };

    bool Empty() {
        std::lock_guard<std::mutex> locker(m_mutex);
        return (m_queue.size() == m_lengthQueueMax);
    };

    bool Full() {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.empty();
    };
    
    size_t Size() {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size();
    };
    
    int32_t Count() {
        return m_queue.size();
    };
    
};

#define NUM_TASK_MAX 1
class ThreadPool {
    using Task = std::function<void()>;
private:
    SyncQueue<Task> m_queue;
    std::list<std::shared_ptr<std::thread>> m_threadGroup;
    std::atomic_bool m_running;//control actual thread loop
    std::once_flag m_flag;
public:
    ThreadPool(int numT = std::thread::hardware_concurrency()) : m_queue(NUM_TASK_MAX) {
        LOG_INFO("create thread num:%d\n", numT);
        StartThreadGroup(numT);
    }
    ~ThreadPool(void) {
        Stop();
    }
    void Stop() {
        std::call_once(m_flag, [this] {StopThreadGroup();});
    }
    void AddTask(Task&& task) {
        m_queue.Put(std::forward<Task>(task));
    }
    void AddTask(const Task&& task) {
        m_queue.Put(task);
    }
private:
    void StartThreadGroup(int numT) {
        m_running = true;
        for (int i = 0; i < numT;++i) {
            m_threadGroup.push_back(std::make_shared<std::thread>(&ThreadPool::RunThread, this));//?
        }
    }
    void StopThreadGroup() {
        m_queue.Stop();
        m_running = false;
        for (auto pThread : m_threadGroup) {
            if (pThread) {
                pThread->join();
            }
        }
        m_threadGroup.clear();
    }
    void RunThread(void) {
        for (;m_running;) {
            std::list<Task> list;
            m_queue.Take(list);
            for (auto task : list) {
                if (!m_running)
                    return;
                task();
            }
        }
    }
};


#endif