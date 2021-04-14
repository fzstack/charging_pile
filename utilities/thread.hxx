#ifndef UTILITIES_THREAD_HXX_
#define UTILITIES_THREAD_HXX_

#include <Thread.h>
#include <utilities/signals.hxx>

class Thread: public rtthread::Thread {
public:
    using rtthread::Thread::Thread;

    int maxUsed();
    bool isCurrent();
protected:
    virtual void run(void *p) override;
public:
    Signals<void()> onRun;
};



#endif /* UTILITIES_THREAD_HXX_ */
