#include <config/bsp.hxx>
#include <components/persistent_storage_preset.hxx>
#include "data_setter.hxx"

using namespace std;
using namespace Things::Decos;

DataSetter::DataSetter(outer_t* outer): Base(outer) {
    inited.onChanged += [this](auto value) {
        if(!value) return;

        for(rt_uint8_t i = 0u; i < Config::Bsp::kPortNum; i++) {
            auto charger = getInfo(InnerPort{i}).charger;
            charger->stateStore->oState += [this, i, charger](auto state) {
                if(!state) return;
                switch(*state) {
                case State::Charging:
                case State::LoadWaitRemove: {
                    auto& prevCurrMiA = specs[i].prevCurrMiA;
                    auto guard = getLock();
                    prevCurrMiA = *charger->multimeterChannel->current.value();
                    specs[i].count.retrigger();
                    }
                    break;
                default:
                    break;
                };
            };

            charger->multimeterChannel->current += [this, i, charger](auto value) {
                if(!value) return;
                auto& prevCurrMiA = specs[i].prevCurrMiA;
                auto& count = specs[i].count;
                if(*charger->stateStore->oState != State::Charging) {
                    return;
                }
                Preset::PersistentStorage::get()->make<Params>([this, i, value, &prevCurrMiA, &count](auto params){
                    auto guard = getLock();
                    if(abs(*value - prevCurrMiA) >= params->currDiffThrMiA) {
                        prevCurrMiA = *value;
                        count.retrigger();
                    }
                });

            };
        }

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

        timer.start();
    };
}

void DataSetter::emitPortData(InnerPort port) {
    auto& info = getInfo(port);
    this->outer->onCurrentData(CurrentData {
        port: port,
        timerId: info.timerId,
        leftMinutes: info.leftSeconds / 60,
        state: info.charger->stateStore->oState.value().value_or(State::Error),
        current: info.charger->multimeterChannel->current.value().value_or(0),
        voltage: info.charger->multimeterChannel->voltage.value().value_or(0),
        consumption: info.consumption,
        fuse: CurrentData::Fuse::Normal,
    });
}

void DataSetter::init() {
    inited = true;
}

void DataSetter::query() {
    auto guard = getLock();
    for(rt_uint8_t i = 0; i < Config::Bsp::kPortNum; i++) {
        specs[i].count.retrigger();
    }
}

void DataSetter::config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) {
    Preset::PersistentStorage::get()->make<Params>([=](auto params) {
        params->currDiffThrMiA = uploadThr;
    });
}
