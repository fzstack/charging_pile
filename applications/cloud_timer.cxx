#include "cloud_timer.hxx"
#include <devices/air724.hxx>
#include <components/air_components.hxx>

CloudTimer::CloudTimer(outer_t* outer, rt_tick_t time, const char* name): nested_t(outer), Timer(time, name) {

}

void CloudTimer::start() {
    Timer::start();
}

void CloudTimer::run() {
    outer->onHeartbeat();
}
