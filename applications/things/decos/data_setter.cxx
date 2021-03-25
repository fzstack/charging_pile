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
//                    auto& prevCurrMiA = specs[i].prevCurrMiA;
//                    auto& prevTick = specs[i].prevTick;
//                    prevTick = rt_tick_get();
//                    prevCurrMiA = *charger->multimeterChannel->current.value();
//                    this->outer->onCurrentData();
                    }
                    break;
                default:
                    break;
                };
            };

            charger->multimeterChannel->current += [this, i, charger](auto value) {
                if(!value) return;
                auto& prevCurrMiA = specs[i].prevCurrMiA;
                auto& prevTick = specs[i].prevTick;
                auto params = Preset::PersistentStorage::get()->make<Params>();
                if(*charger->stateStore->oState == State::Charging) {
                    if(abs(*value - prevCurrMiA) >= params->currDiffThrMiA) {
                        if(rt_tick_get() - prevTick > 10000) {
                            prevTick = rt_tick_get();
                            //TODO: 上报这个端口的状态
                            prevCurrMiA = *value;
                        }
                    }
                }
            };
        }
    };
}

void DataSetter::init() {
    inited = true;
}

void DataSetter::config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) {
    auto params = Preset::PersistentStorage::get()->make<Params>();
    params->currDiffThrMiA = uploadThr;
}
