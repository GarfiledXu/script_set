#ifndef _TIMER_HPP
#define _TIMER_HPP

#include "main.hpp"
// #include "MyFlags.hpp"
#include "sdk-pkg.hpp"
#include "threadpool.hpp"
class timer_thread
{
private:
    /* data */
    std::thread m_thread;
public:
    timer_thread(/* args */);
    ~timer_thread();
};

// timer_thread(timing_time_ms, func, arg...);
timer_thread::timer_thread(){
    
}

timer_thread::~timer_thread() {
    
}



#endif