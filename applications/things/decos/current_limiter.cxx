#include <components/persistent_storage_preset.hxx>
#include <config/app.hxx>
#include <Lock.h>
#include "current_limiter.hxx"

using namespace std;
using namespace Things::Decos;

#define LOG_CL_CONF

CurrentLimiter::CurrentLimiter(outer_t* outer): Base(outer) {
    timer.onRun += [this]() {
        {
            auto guard = getLock();
            params.refresh();
        }
        for(rt_uint8_t i = 0u; i < Config::Bsp::kPortNum; i++) {
            auto willStop = false;
            auto charger = getInfo(InnerPort{i}).charger;
            {
                auto guard = getLock();
                willStop = specs[i].count.updateAndCheck();
            }
            if(willStop) {
                charger->stop();
                this->outer->onCurrentLimit(InnerPort{i});
            }
        }
    };
}

void CurrentLimiter::init() {
    timer.start();
}

void CurrentLimiter::onCurrentChanged(InnerPort port, int value) {
    auto guard = getLock();
    auto charger = getInfo(port).charger;
    auto& count = specs[port.get()].count;
    if(charger->stateStore->oState.value() != State::Charging || params == nullopt) {
        count.reset();
        return;
    }
#ifdef LOG_CL_CONF
    rt_kprintf("[%d] cur: %dmA\n", NatPort{port}.get(), value);
#endif
    if(value > params->maxCurrentMiA) {
        count.trigger();
    } else {
        count.reset();
    }
}

void CurrentLimiter::config(DevConfig conf) {
    params.save({conf.currentLimit});
}

