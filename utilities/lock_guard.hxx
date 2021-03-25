#ifndef UTILITIES_LOCK_GUARD_HXX_
#define UTILITIES_LOCK_GUARD_HXX_

#include <rtthread.h>

//TODO: DEPRESSED
class LockGuard {
public:
    LockGuard(rt_mutex_t mutex): mutex(mutex) {
        rt_mutex_take(mutex, RT_WAITING_FOREVER);
    }

    ~LockGuard() {
        rt_mutex_release(mutex);
    }

private:
    rt_mutex_t mutex;
};



#endif /* UTILITIES_LOCK_GUARD_HXX_ */
