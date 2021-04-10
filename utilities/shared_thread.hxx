#ifndef UTILITIES_SHARED_THREAD_HXX_
#define UTILITIES_SHARED_THREAD_HXX_

#include "thread.hxx"
#include "post.hxx"
#include "signals.hxx"
#include <config/os.hxx>

class SharedThread: public ::Thread {
    using ::Thread::Thread;
public:
    template<class... Args>
    void exec(Args&&... args) {
        runOn(post(std::forward<Args>(args)...));
    }
protected:
    virtual void run(void *p) override;
public:
    Post post;
};

#include <utilities/singleton.hxx>
#include <config/bsp.hxx>
namespace Preset {
template<Priority Prio>
class SharedThread: public Singleton<SharedThread<Prio>>, public ::SharedThread {
    friend class Singleton<SharedThread<Prio>>;
    SharedThread(): ::SharedThread(kThreadStack, PriorityTrait<Prio>::value, kThreadTick, kThread) {
        start();
    }
    static const int kThreadStack, kThreadTick;
public:
    static const char *kThread;
};
}

#endif /* UTILITIES_SHARED_THREAD_HXX_ */
