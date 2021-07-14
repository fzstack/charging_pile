#include <config/bsp.hxx>
#include "data_setter.hxx"

using namespace std;
using namespace Things::Decos;

DataSetter::DataSetter(outer_t* outer): Base(outer) {
    timer.onRun += [this] {
        auto willEmit = false;
        auto port = rt_uint8_t{};
        {
            auto guard = getLock();
            port = currPort;
            willEmit = specs[port].count.updateAndCheck();
            currPort++;
            currPort %= Config::Bsp::kPortNum;
        }
        if(willEmit) {
            emitPortData(InnerPort{port});
        }
    };
}

void DataSetter::emitPortData(InnerPort port) {
    auto& info = getInfo(port);
    auto& spec = specs[port.get()];
    auto state = spec.prevState;
    float consumption;

    auto vState = State::Error;
    if(state == State::LoadWaitRemove) {
        vState = info.finalState;
        consumption = spec.prevConsumption;
        rt_kprintf("final state: %d\n", vState);
    } else {
        vState = state;
        consumption = info.consumption;
    }

    rt_kprintf("emit port %d, s: %d\n", NatPort{port}.get(), vState);

    this->outer->onCurrentData(CurrentData {
        port: port,
        timerId: info.timerId,
        leftMinutes: info.leftSeconds / 60,
        state: vState,
        current: info.charger->multimeterChannel->current.value().value_or(0),
        voltage: info.charger->multimeterChannel->voltage.value().value_or(0),
        consumption: consumption,
        fuse: CurrentData::Fuse::Normal,
    });
    if(state == State::LoadWaitRemove) {
        //清空
        info.timerId = 0;
        info.consumption = 0;
        info.finalState = State::LoadNotInsert;
        spec.prevState = State::LoadNotInsert;
    }
}

void DataSetter::init() {
    timer.start();
}

void DataSetter::query() {
    auto guard = getLock();
    for(rt_uint8_t i = 0; i < Config::Bsp::kPortNum; i++) {
        specs[i].count.retrigger();
    }
}

void DataSetter::onStateChanged(InnerPort port, State::Value state) {
    auto& info = getInfo(port);
    auto charger = info.charger;
    auto& spec = specs[port.get()];
    auto guard = getLock();
    switch(state) {
    case State::Charging:
        info.finalState = State::LoadWaitRemove;
        spec.prevCurrMiA = charger->multimeterChannel->current.value().value_or(0);
        spec.prevState = state;
        spec.count.retrigger();
        break;
    case State::LoadWaitRemove:
        spec.prevState = state;
        spec.prevConsumption = info.consumption;
        spec.count.retrigger();
        break;
    default:
        break;
    };
}

void DataSetter::onCurrentChanged(InnerPort port, int value) {
    auto charger = getInfo(port).charger;
    auto& spec = specs[port.get()];
    auto& prevCurrMiA = spec.prevCurrMiA;
    auto guard = getLock();
    if(*charger->stateStore->oState != State::Charging) {
        return;
    }
    if(abs(value - prevCurrMiA) >= getConfig().uploadThr) {
        prevCurrMiA = value;
        spec.count.retrigger();
    }
}
