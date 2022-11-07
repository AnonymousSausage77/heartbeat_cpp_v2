#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <functional>
#include <iterator>
#include <memory>
#include <vector>
namespace roboseals
{

struct Observer
{
    std::function<void(int32_t, const std::string&)> _t;
    Observer() = delete;
    Observer(std::function<void(int32_t, const std::string&)> t) : _t(t) {}
    auto run(int32_t signal, const std::string &message) {
        return _t(signal, message);
    }
    Observer operator=(std::function<void(int32_t, const std::string&)> t) {
        return Observer(t);
    }
    
};

template<typename O>
inline void update(std::vector<std::weak_ptr<O>> list, int32_t signal, const std::string &message)
{
    for (auto itr = list.begin(); itr != list.end(); itr ++) {
        if (!(*itr).expired()) 
        {
            // run the observer
            itr->lock()->run(signal, message); 
        } 
        else 
        { 
            // remove the observer from list of observers
            list.erase(itr); 
        };
    }
}


}

#endif // OBSERVER_HPP
