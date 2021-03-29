#include <components/persistent_storage_preset.hxx>
#include <config/app.hxx>
#include <Lock.h>
#include "current_limiter.hxx"

using namespace std;
using namespace rtthread;
using namespace Things::Decos;

CurrentLimiter::CurrentLimiter(outer_t* outer): Base(outer), mutex(kMutex) {
    inited.onChanged += [this](auto value) {
        if(!value) return;

        timer.onRun += [this]() {
            auto guard = Lock(mutex);
            for(rt_uint8_t i = 0u; i < Config::Bsp::kPortNum; i++) {
                if(specs[i].updateAndCheck()) {
                    auto charger = getInfo(InnerPort{i}).charger;
                    charger->stop();
                    this->outer->onCurrentLimit(InnerPort{i});
                }
            }
        };
        timer.start();

        for(rt_uint8_t i = 0u; i < Config::Bsp::kPortNum; i++) {
            auto charger = getInfo(InnerPort{i}).charger;

            charger->multimeterChannel->current += [this, charger, i](auto value) {
                auto guard = Lock(mutex);
                if(!value) return;

                if(charger->stateStore->oState.value() != State::Charging) {
                    specs[i].reset();
                    return;
                }
                rt_kprintf("port%d current: %dmA\n", i, *value);
                auto storage = Preset::PersistentStorage::get();
                storage->make<Params>([this, value, i](auto params) {
                    auto guard = Lock(mutex);
                    if(*value > params->maxCurrentMiA) {
                        specs[i].trigger();
                    } else {
                        specs[i].reset();
                    }
                });
            };
        }
    };
}

void CurrentLimiter::init() {
    inited = true;
}

void CurrentLimiter::config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) {
    Preset::PersistentStorage::get()->make<Params>([=](auto params) {
        params->maxCurrentMiA = currentLimit;
    });
}

