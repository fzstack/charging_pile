#include "consumption_measurer.hxx"
#include <Lock.h>

using namespace std;
//using namespace rtthread;
using namespace Things::Decos;

ConsumptionMeasurer::ConsumptionMeasurer(outer_t* outer): Base(outer) {
    timer.onRun += [this] {
        for(rt_uint8_t i = 0; i < Config::Bsp::kPortNum; i++) {
            auto& info = getInfo(InnerPort{rt_uint8_t(i)});
            auto& spec = specs[i];
            update(info, spec);
        }
    };
}

void ConsumptionMeasurer::init() {
    timer.start();
}

void ConsumptionMeasurer::onStateChanged(InnerPort port, State::Value state) {
    auto& info = getInfo(port);
    auto& spec = specs[port.get()];
    auto guard = getLock();
    switch(state) {
    case State::Charging: //开始充电后耗电量清零，开启定时器
        info.consumption = 0;
        spec.tick = rt_tick_get();
        spec.prevCurr = info.charger->multimeterChannel->current.value().value_or(0);
        spec.prevVol = info.charger->multimeterChannel->voltage.value().value_or(0);
        break;
    case State::LoadWaitRemove: //充电完成，关闭定时器  TODO: consumption也写入需保存的状态中
        //info.consumption = 0;
        break;
    case State::LoadNotInsert: //负载拔出后耗电量清零
        info.consumption = 0;
        break;
    default:
        break;
    }
}

void ConsumptionMeasurer::onCurrentChanged(InnerPort port, int value) {
    auto& info = getInfo(port);
    auto& spec = specs[port.get()];
    update(info, spec);
}

void ConsumptionMeasurer::onVoltageChanged(InnerPort port, int value) {
    auto& info = getInfo(port);
    auto& spec = specs[port.get()];
    update(info, spec);
}

void ConsumptionMeasurer::update(ChargerInfo& info, ChargerSpec& spec) {
    auto guard = getLock();
    if(*info.charger->stateStore->oState != State::Charging) return;
    info.consumption += spec.prevCurr / 1000.f * spec.prevVol * (rt_tick_get() - spec.tick) / 1000.f;
    spec.tick = rt_tick_get();
    spec.prevCurr = info.charger->multimeterChannel->current.value().value_or(0);
    spec.prevVol = info.charger->multimeterChannel->voltage.value().value_or(0);
}
