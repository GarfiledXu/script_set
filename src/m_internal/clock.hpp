#ifndef _CLOCK_HPP
#define _CLOCK_HPP


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