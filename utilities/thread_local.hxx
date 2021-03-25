#ifndef UTILITIES_THREAD_LOCAL_HXX_
#define UTILITIES_THREAD_LOCAL_HXX_

#include <map>
#include <rtthread.h>
#include <Mutex.h>
#include <Lock.h>
#include <tuple>
#include <memory>
#include <unordered_map>

template<class T>
class ThreadLocal {
public:
    ThreadLocal(): pt() {

    }

    ThreadLocal(T&& t): pt(std::make_shared<T>(t)) {

    }

    T& operator->() {
        return get();
    }

    T& operator*() {
        return get();
    }

    void operator=(const T& other) {
        auto guard = rtthread::Lock(mutex);
        auto selfThread = rt_thread_self();
        m[selfThread] = other;
    }

    void operator=(T&& other) {
        auto guard = rtthread::Lock(mutex);
        auto selfThread = rt_thread_self();
        m[selfThread] = std::move(other);
    }

private:
    T& get() {
        auto guard = rtthread::Lock(mutex);
        auto selfThread = rt_thread_self();
        auto found = m.find(selfThread);
        if(found == m.end()) {
            if(pt != nullptr) {
                m[selfThread] = T{*pt};
            } else {
                m[selfThread] = T{};
            }

            found = m.find(selfThread);
        }
        return found->second;
    }

private:
    std::shared_ptr<T> pt;
    std::unordered_map<rt_thread_t, T> m = {};
    static rtthread::Mutex mutex;
};

template<class T> rtthread::Mutex ThreadLocal<T>::mutex = {};

#endif /* UTILITIES_THREAD_LOCAL_HXX_ */
