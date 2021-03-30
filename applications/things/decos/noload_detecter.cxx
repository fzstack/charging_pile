#include <config/bsp.hxx>
#include <components/persistent_storage_preset.hxx>
#include "noload_detecter.hxx"

using namespace Things::Decos;
using namespace std;

#define LOG_NLD_DET_CONF
#define LOG_NLD_SET_CUR

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

            charger->multimeterChannel->current += [this, charger, i, &spec](auto value) {
                if(!value) return;

                auto guard = getLock();
                if(*charger->stateStore->oState != State::Charging) {
                    return;
                }
//                if(params == nullopt) {
//                    return;
//                }
//
//#ifdef LOG_NLD_SET_CUR
//                rt_kprintf("[%d] cur: %dmA\n", NatPort{InnerPort{i}}.get(), *value);
//#endif
//
//                if(*value < params->noloadCurrThr) {
//                    spec.count.trigger();
//                } else {
//                    spec.count.reset();
//                }
            };
        }

        timer.onRun += [this]{
            //params.refresh();

            auto port = currPort;
            currPort++;
            currPort %= Config::Bsp::kPortNum;
            auto& info = getInfo(InnerPort{port});
            auto& spec = specs[port];

            if(spec.count.updateAndCheck()) {
                auto charger = info.charger;
                if(*charger->stateStore->oState == State::Charging) {
                    rt_kprintf("stop chargering due to no-load\n");
                    charger->stop();
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
    //params.save({noloadCurrThr});
}

