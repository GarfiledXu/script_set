#ifndef SYNC_HPP
#define SYNC_HPP


#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <pthread.h> 
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include <thread>
#include <future>
#include <list>

#include <atomic>

//contain mutex
template<typename T>
class SyncBuffBase {
private:
    std::mutex m_mutex;
    T* m_buff;
public:
    T get(void) {
        std::unique_lock<std::mutex> locker(m_mutex, std::defer_lock);
        locker.lock();
        return *m_buff;
    };
    void set(const T& buff) {
        std::unique_lock<std::mutex> locker(m_mutex, std::defer_lock);
        locker.lock();
        *m_buff = buff;
    }
    void set(const SyncBuffBase& sync_base) {
        *m_buff = *(sync_base.m_buff);
    }
    SyncBuffBase() : m_buff(new T()) {};
    SyncBuffBase(const T& buff) : m_buff(new T(buff)) {};
    SyncBuffBase(const SyncBuffBase& sync_base) : m_buff(new T(*(sync_base.m_buff))) {};
    SyncBuffBase& operator=(const SyncBuffBase& sync_base){
        m_buff = new T(*(sync_base.m_buff));
        return *this;
    };
    SyncBuffBase(SyncBuffBase&& sync_base) :m_buff(sync_base.m_buff) {
        if (sync_base.m_buff)
            sync_base.m_buff = NULL;
    };
    SyncBuffBase&& operator=(SyncBuffBase&& sync_base) {
        m_buff = sync_base.m_buff;
        if (this != &sync_base) {
            if (sync_base.m_buff)
                sync_base.m_buff = NULL;
        }
        return *this;
    };
    ~SyncBuffBase() { delete m_buff; };
};

typedef struct
{
    std::string path;
}PicInfoBase;

typedef struct PicInfo : public PicInfoBase
{
    
}PicInfo;



class tClock {
    
private:
    std::chrono::time_point<std::chrono::system_clock> g_last_time;
    // std::unique_ptr<std::chrono::time_point<std::chrono::system_clock>> g_last_time;
public:
    enum PERO {
        SEC,
        MILLISEC,
        MICROSEC,
        NANOSEC
    };
    tClock()  {
        g_last_time = std::chrono::system_clock::now();
    }
    long long get_duration(const PERO& preo) {
        auto end = std::chrono::system_clock::now();
        switch (preo)
        {
        case SEC: {
            auto dur = std::chrono::duration_cast<std::chrono::duration<long long, std::ratio<1,1>>>(end - g_last_time).count();
             g_last_time = end;
            return dur;
        }
        case MILLISEC: {
            auto dur = std::chrono::duration_cast<std::chrono::duration<long long, std::ratio<1,1000>>>(end - g_last_time).count();
             g_last_time = end;
            return dur;
        }
        case MICROSEC: {
            auto dur = std::chrono::duration_cast<std::chrono::duration<long long, std::ratio<1,1000000>>>(end - g_last_time).count();
             g_last_time = end;
            return dur;
        }
        case NANOSEC: {
            auto dur = std::chrono::duration_cast<std::chrono::duration<long long, std::ratio<1,1000000000>>>(end - g_last_time).count();
             g_last_time = end;
            return dur;
        }
        default:
            break;
        }

        g_last_time = end;
        return double(-10);
    }
    void reset_start() {
        g_last_time = std::chrono::system_clock::now();
    }
    ~tClock() {};
};

#endif