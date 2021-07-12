#pragma once
#include <memory>

template<class T>
class WeakSingleton {
public:
    using singleton_t = WeakSingleton<T>;
    template<class... A>
    static std::shared_ptr<T> get(A&&... a) {
        static auto inst = std::weak_ptr<T>();
        auto currInst = std::shared_ptr<T>(nullptr);
        if(inst.expired()) {
            currInst = std::shared_ptr<T>(new T(std::forward<A>(a)...));
            inst = currInst;
        } else {
            currInst = inst.lock();
        }
        return currInst;
    }
};