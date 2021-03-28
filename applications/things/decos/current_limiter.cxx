#include <components/persistent_storage_preset.hxx>
#include <config/app.hxx>
#include <Lock.h>
#include "current_limiter.hxx"

using namespace std;
using namespace rtthread;
using namespace Things::Decos;

CurrentLimiter::CurrentLimiter(outer_t* outer): Base(outer), mutex(kMutex) {

    //每个端口一个定时器, TODO: 可优化
    for(auto& timer: timers) {
        timer = make_shared<Timer>(kDuration, kTimer, RT_TIMER_FLAG_ONE_SHOT);
    }

    inited.onChanged += [this](auto value) {
        if(!value) return;
        for(rt_uint8_t i = 0u; i < Config::Bsp::kPortNum; i++) {
            auto charger = getInfo(InnerPort{i}).charger;
            auto timer = timers[i];

            charger->multimeterChannel->current += [this, charger, timer, i](auto value) {
                auto guard = Lock(mutex);
                if(!value) return;

                rt_kprintf("port%d current: %dmA\n", i, *value);
                if(charger->stateStore->oState.value() != State::Charging) return;

                auto storage = Preset::PersistentStorage::get();
                rt_kprintf("port%d charing limit detecting\n", i);
                storage->make<Params>([=](auto params) {
                    if(*value > params->maxCurrentMiA) {
                        if(!timer->isRunning() && charger->stateStore->oState.value() == State::Charging) timer->start();
                    } else {
                        if(timer->isRunning()) timer->stop();
                    }
                });
            };

            timer->onRun += [charger, i, this]() {
                auto guard = Lock(mutex);
                if(charger->stateStore->oState.value() == State::Charging) {
                    charger->stop();
                    this->outer->onCurrentLimit(InnerPort{i});
                }
            };
        }
    };
}

void CurrentLimiter::init() {
    inited = true;
}

void CurrentLimiter::config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) {
    auto params = Preset::PersistentStorage::get()->make<Params>();
    params->maxCurrentMiA = currentLimit;
}

