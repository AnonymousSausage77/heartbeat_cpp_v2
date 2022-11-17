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
/**
 * @class Monitor
 * @author Coding
 * @date 17/11/2022
 * @file Monitor.hpp
 * @brief Monitor class that wraps a variable and provides a threadsafe means to access it. When variables are accessed
 *  through the '->' operator, this class provides threadsafe access for the duration of the member's access.
 * For example, foo->test(); will provide thread-safety for the duration of test(). After test() finishes, the lock is
 * released. Additionally, this also provides a scope-duration lock through the method: .manuallyLock();
 */ 
class Monitor
{
public:
    template<typename ...Args>
    Monitor(Args&&... args) : m_cl(std::forward<Args>(args)...){}

    struct Monitor_helper
    {
        Monitor_helper(Monitor* mon) : m_mon(mon), m_ul(mon->m_lock) { locks++; /*std::cout << "lcked "<< locks << std::endl;*/ }
        ~Monitor_helper() { /*std::cout << "unlcked " << locks << std::endl;*/ locks--; };
        T* operator->() { return &m_mon->m_cl;}
        Monitor* m_mon;
        std::unique_lock<std::mutex> m_ul;
    };
    
    void operator>>(std::function<void(T&)> &syncFunc) { Monitor_helper monitor{this}; syncFunc(m_cl); } // synchronised
    /**
     * @brief Provides 1-time, thread safe access to the target method. 
     */
    Monitor_helper operator->() { return Monitor_helper(this); }
    /**
     * @brief Creates a Monitor_helper that locks the contained resource for as long as it exists, unlocking the resource
     * within the Monitor_helper's destructor.
     * of locking a resource for as long as you need it.
     */
    Monitor_helper manuallyLock() { return Monitor_helper(this); }
    T& getThreadUnsafeAccess() { return m_cl; }

private:
    T           m_cl;
    std::mutex  m_lock;
};

}

#endif // MONITOR_HPP
