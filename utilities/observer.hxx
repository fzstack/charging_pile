#ifndef UTILITIES_OBSERVER_HXX_
#define UTILITIES_OBSERVER_HXX_

#include "signals.hxx"
#include "observable.hxx"
#include <functional>
template <class T>
struct Observable;

template<class T>
class Observer {
public:
    Observer(Observable<T>& obs): obs(obs) { }

    void operator+=(std::function<void(const T& value)> cb) {
        obs.onChanged += cb;
    }

    T value() const {
        return obs;
    }

    T operator*() const {
        return obs;
    }

public:
    Observable<T>& obs;
};



#endif /* UTILITIES_OBSERVER_HXX_ */
