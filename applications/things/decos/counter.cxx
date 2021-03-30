#include <config/bsp.hxx>
#include "counter.hxx"

using namespace Things::Decos;

Counter::Counter(outer_t* outer): Base(outer) {
    timer.onRun += [this](){
        auto guard = getLock();
        for(rt_uint8_t i = 0u; i < Config::Bsp::kPortNum; i++) {
            auto& info = getInfo(InnerPort{i});
            auto& charger = info.charger;
            auto& leftSeconds = info.leftSeconds;
            if(*charger->stateStore->oState == State::Charging) {
                if(--leftSeconds <= 0) {
                    charger->stop();
                }
                rt_kprintf("[%d] remains %ds\n", NatPort{InnerPort{i}}.get(), leftSeconds);
            }
        }
    };
}

void Counter::init() {
    timer.start();
}

void Counter::onStateChanged(InnerPort port, State::Value state) {
    auto guard = getLock();
    auto& info = getInfo(port);
    if(state != State::LoadWaitRemove) return;
    info.leftSeconds = 0; //充电完成则将剩余时间清零
}

