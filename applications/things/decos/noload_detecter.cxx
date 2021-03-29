#include <config/bsp.hxx>
#include <components/persistent_storage_preset.hxx>
#include "noload_detecter.hxx"

using namespace Things::Decos;

NoloadDetecter::NoloadDetecter(outer_t* outer): Base(outer) {
    inited.onChanged += [this](auto value) {
        if(!value) return;
        for(rt_uint8_t i = 0u; i < Config::Bsp::kPortNum; i++) {
            auto& info = getInfo(InnerPort{i});
            auto& spec = specs[i];
            auto charger = info.charger;

            charger->stateStore->oState += [&spec](auto state) {
                if(!state) return;
                if(state != State::Charging) {
                    spec.count.reset();
                }
            };

            charger->multimeterChannel->current += [charger, &spec](auto value) {
                if(!value) return;
                if(*charger->stateStore->oState != State::Charging) return;
                Preset::PersistentStorage::get()->make<Params>([value, &spec](auto params){
                    if(*value < params->noloadCurrThr) {
                        spec.count.trigger();
                    } else {
                        spec.count.reset();
                    }
                });

            };
        }

        timer.onRun += [this]{
            for(rt_uint8_t i = 0; i < Config::Bsp::kPortNum; i++) {
                auto& info = getInfo(InnerPort{i});
                auto& spec = specs[i];

                if(spec.count.updateAndCheck()) {
                    auto charger = info.charger;
                    if(*charger->stateStore->oState == State::Charging) {
                        rt_kprintf("stop chargering due to no-load\n");
                        charger->stop();
                    }
                }
            }
        };

        timer.start();
    };
}

void NoloadDetecter::init() {
    inited = true;
}

void NoloadDetecter::config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) {
    Preset::PersistentStorage::get()->make<Params>([=](auto params) {
        params->noloadCurrThr = noloadCurrThr;
    });
}
