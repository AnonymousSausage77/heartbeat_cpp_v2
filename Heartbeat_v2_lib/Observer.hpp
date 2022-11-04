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
    std::function<void()> _t;
    Observer(std::function<void()> t) : _t(t) {}
    auto run() {
        return _t();
    }
    
};

template<typename O>
inline void update(std::vector<std::weak_ptr<O>> list)
{
    for (auto itr = list.begin(); itr != list.end(); itr ++) {
        if (!(*itr).expired()) 
        {
            // run the observer
            itr->lock()->run(); 
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
