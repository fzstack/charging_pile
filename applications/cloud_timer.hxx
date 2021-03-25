#ifndef APPLICATIONS_CLOUD_TIMER_HXX_
#define APPLICATIONS_CLOUD_TIMER_HXX_

#include "cloud.hxx"
#include <components/timer.hxx>
#include <utilities/nested.hxx>
#include <utilities/signals.hxx>

class Cloud;
namespace Preset {
class CloudTimer;
}

class CloudTimer: public Nested<Cloud>, public Timer {
    friend outer_t;
    friend class Preset::CloudTimer;
private:
    CloudTimer(outer_t* outer, rt_tick_t time, const char* name);
public:
    virtual void start() override;
protected:
    virtual void run() override;
private:
    //Signals<void()> signal = {};
};

#include <utilities/singleton.hxx>
namespace Preset {
class CloudTimer: public Singleton<CloudTimer>, public ::CloudTimer {
    friend class Singleton<CloudTimer>;
    CloudTimer(::CloudTimer::outer_t* outer): ::CloudTimer(outer, kTime, kName) {}
    static const rt_tick_t kTime;
    static const char *kName;
};
}



#endif /* APPLICATIONS_CLOUD_TIMER_HXX_ */
