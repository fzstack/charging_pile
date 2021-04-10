#include <config/bsp.hxx>
#include <components/persistent_storage_preset.hxx>
#include "noload_detecter.hxx"
#include <algorithm>

using namespace Things::Decos;
using namespace std;

#define LOG_NLD_SET_CUR

NoloadDetecter::NoloadDetecter(outer_t* outer): Base(outer) {
    timer.onRun += [this]{
        params.refresh();
        auto port = currPort;
        currPort++;
        currPort %= Config::Bsp::kPortNum;
        auto& info = getInfo(InnerPort{port});
        auto charger = info.charger;
        auto& spec = specs[port];
        auto willStop = false;
        auto noloadCond = false;
        {
            auto guard = getLock();
            noloadCond = spec.noloadCount.updateAndCheck();
            auto doneCond = spec.doneCount.updateAndCheck();
            willStop = (noloadCond || doneCond) && *charger->stateStore->oState == State::Charging;
        }
        if(willStop) {
            if(noloadCond) info.finalState = State::LoadNotInsert;
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
        spec.noloadCount.reset();
        spec.doneCount.reset();
    } else {
        auto charger = getInfo(port).charger;
        auto curr = charger->multimeterChannel->current.value().value_or(0);
        checkCurrent(port, curr);
    }
}

void NoloadDetecter::onCurrentChanged(InnerPort port, int value) {
    auto charger = getInfo(port).charger;
    auto guard = getLock();
    if(charger->stateStore->oState.value() != State::Charging)
        return;
    checkCurrent(port, value);
}

void NoloadDetecter::checkCurrent(InnerPort port, int value) {
    auto& spec = specs[port.get()];
    if(params == nullopt) {
        return;
    }
#ifdef LOG_NLD_SET_CUR
    rt_kprintf("[%d] cur: %dmA, noloadCurrThr: %dmA\n", NatPort{port}.get(), value, params->noloadCurrThr);
#endif

    if(value < params->noloadCurrThr) {
        rt_kprintf("[%d] noload trigger\n", NatPort{port}.get());
        spec.noloadCount.trigger();
    } else {
        rt_kprintf("[%d] noload reset\n", NatPort{port}.get());
        spec.noloadCount.reset();
    }

    if(value < params->doneCurrThr) {
        spec.doneCount.trigger();
    } else {
        spec.doneCount.reset();
    }
}

void NoloadDetecter::config(DevConfig conf) {
    params.save({conf.noloadCurrThr, conf.doneCurrThr});
}

