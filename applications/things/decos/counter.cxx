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

    inited.onChanged += [this](auto value) {
        if(!value) return;
        for(rt_uint8_t i = 0u; i < Config::Bsp::kPortNum; i++) {
            auto& info = getInfo(InnerPort{i});
            info.charger->stateStore->oState += [this, &info](auto value) {
                auto guard = getLock();
                if(value != State::LoadWaitRemove) return;
                info.leftSeconds = 0; //充电完成则将剩余时间清零
            };
        }
        timer.start();
    };
}

void Counter::init() {
    inited = true;
}

