#ifndef COMPONENTS_TIMER_HXX_
#define COMPONENTS_TIMER_HXX_

#include <rtthread.h>
#include <memory>
#include <utilities/signals.hxx>

class Timer {
public:
    Timer(rt_tick_t time, const char* name, rt_uint8_t flags = RT_TIMER_FLAG_PERIODIC);
    virtual void start();
    virtual void stop();
    bool isRunning();
    int getDuration();
protected:
    virtual void run();
public:
    Signals<void()> onRun = {};
private:
    std::shared_ptr<rt_timer> timer;
    const rt_tick_t duration;
    bool running = false;
    rt_uint8_t flags;
};

#endif
