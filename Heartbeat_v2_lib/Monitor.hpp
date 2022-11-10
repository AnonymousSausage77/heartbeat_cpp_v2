#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <thread>
#include <mutex>
#include <functional>
#include <iostream>

namespace roboseals
{

static int locks = 0;
    
// from: https://stackoverflow.com/questions/12647217/making-a-c-class-a-monitor-in-the-concurrent-sense
template<class T>
class Monitor
{
public:
    template<typename ...Args>
    Monitor(Args&&... args) : m_cl(std::forward<Args>(args)...){}

    struct Monitor_helper
    {
        Monitor_helper(Monitor* mon) : m_mon(mon), m_ul(mon->m_lock) { locks++; std::cout << "lcked "<< locks << std::endl; }
        ~Monitor_helper() { std::cout << "unlcked " << locks << std::endl; locks--; };
        T* operator->() { return &m_mon->m_cl;}
        Monitor* m_mon;
        std::unique_lock<std::mutex> m_ul;
    };
    
    void operator>>(std::function<void(T&)> &syncFunc) { Monitor_helper monitor{this}; syncFunc(m_cl); } // synchronised
            // block of code
    Monitor_helper operator->() { return Monitor_helper(this); }
    Monitor_helper manuallyLock() { return Monitor_helper(this); }
    T& getThreadUnsafeAccess() { return m_cl; }

private:
    T           m_cl;
    std::mutex  m_lock;
};

}

#endif // MONITOR_HPP
