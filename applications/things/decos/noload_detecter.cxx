#include <config/bsp.hxx>
#include <components/persistent_storage_preset.hxx>
#include "noload_detecter.hxx"
#include <algorithm>

using namespace Things::Decos;
using namespace std;

//#define LOG_NLD_SET_CUR

NoloadDetecter::NoloadDetecter(outer_t* outer): Base(outer) {
    timer.onRun += [this]{
        params.refresh();
        auto port = currPort;
        currPort++;
        currPort %= Config::Bsp::kPortNum;
        auto charger = getInfo(InnerPort{port}).charger;
        auto& spec = specs[port];
        auto willStop = false;
        {
            auto guard = getLock();
            willStop = spec.count.updateAndCheck() && *charger->stateStore->oState == State::Charging;
        }
        if(willStop) {
            charger->stop();
            rt_kprintf("stop chargering due to no-load\n");
        }
    };
}

void NoloadDetecter::init() {
    timer.start();
}

void NoloadDetecter::onStateChanged(InnerPort port, State::Value state) {
    auto& spec = specs[port.get()];
    auto guard = getLock();
    if(state != State::Charging) {
        spec.count.reset();
    }
}

void NoloadDetecter::onCurrentChanged(InnerPort port, int value) {
    auto charger = getInfo(port).charger;
    auto& spec = specs[port.get()];
    auto guard = getLock();
    if(charger->stateStore->oState.value() != State::Charging) {
        return;
    }
    if(params == nullopt) {
        return;
    }

#ifdef LOG_NLD_SET_CUR
    rt_kprintf("[%d] cur: %dmA, noloadCurrThr: %dmA\n", NatPort{InnerPort{i}}.get(), *value, params->noloadCurrThr);
#endif


    if(value < max(params->noloadCurrThr, params->doneCurrThr)) {
        spec.count.trigger();
    } else {
        spec.count.reset();
    }
}

void NoloadDetecter::config(DevConfig conf) {
    params.save({conf.noloadCurrThr, conf.doneCurrThr});
}

