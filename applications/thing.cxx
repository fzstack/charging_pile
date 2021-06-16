#include "thing.hxx"
#include <Lock.h>
#include <optional>

using namespace std;
using namespace string_literals;
using namespace rtthread;

Thing::Thing(array<shared_ptr<Charger>, Config::Bsp::kPortNum> chargers):
    infos() {

    transform(chargers.begin(), chargers.end(), infos.begin(), [](auto charger) {
        return ChargerInfo {
            charger: charger,
        };
    });

    inited.onChanged += [this](auto value) {
        if(!value) return;
        for(auto i = 0u; i < this->infos.size(); i++) {
            auto& info =  this->infos[i];
            auto charger = info.charger;
            charger->init();
        }
        for(auto& deco: decos) {
            deco->init();
        }

        for(rt_uint8_t i = 0; i < Config::Bsp::kPortNum; i++) {
            auto charger = this->infos[i].charger;
            charger->stateStore->oState += [this, i](auto state) {
                if(!state) return;
                for(auto& deco: decos) {
                    deco->onStateChanged(InnerPort{i}, *state);
                }
            };

            charger->multimeterChannel->current += [this, i](auto value) {
                if(!value) return;
                for(auto& deco: decos) {
                    deco->onCurrentChanged(InnerPort{i}, *value);
                }
            };

            charger->multimeterChannel->voltage += [this, i](auto value) {
                if(!value) return;
                for(auto& deco: decos) {
                    deco->onVoltageChanged(InnerPort{i}, *value);
                }
            };

            auto state = charger->stateStore->oState.value();
            if(state != nullopt) {
                for(auto& deco: decos) {
                    deco->onStateChanged(InnerPort{i}, *state);
                }
            }
        }
    };
}

void Thing::init() {
    inited = true;
}

void Thing::query() {
    for(auto& deco: decos) {
        deco->query();
    }
}

void Thing::control(InnerPort port, int timerId, int minutes) {
    auto guard = Lock(mutex);
    auto& info = infos[port.get()]; //NOTE: std::array会自动进行边界检查
    auto oldTimerId = info.timerId;
    info.leftSeconds = minutes * 60;
    info.timerId = timerId;
    try {
        info.charger->start();
    } catch(const exception& e) {
        info.leftSeconds = 0;
        info.timerId = oldTimerId;
    }
}

void Thing::stop(InnerPort port, int timerId) {
    auto& info = infos[port.get()];
    info.charger->stop();
    info.timerId = timerId;
}

void Thing::config(DevConfig conf) {
    for(auto deco: decos) {
        deco->config(conf);
    }
}

DevConfig Thing::readConfig() {
    throw not_implemented{"read conf in upper end pls"};
}

#ifdef LOWER_END
#include <things/decos/counter.hxx>
#include <things/decos/current_limiter.hxx>
#include <things/decos/backuper.hxx>
#include <things/decos/data_setter.hxx>
#include <things/decos/consumption_measurer.hxx>
#include <things/decos/noload_detecter.hxx>
using namespace Things::Decos;
namespace Preset {
ThingPre::ThingPre(): ::Thing(Chargers::get()) {
    addDeco<Counter>(); //定时断电功能
    addDeco<CurrentLimiter>(); //限流功能
    addDeco<NoloadDetecter>(); //空载检测功能
    addDeco<Backuper>(); //端口状态备份功能
    addDeco<DataSetter>(); //端口状态上报功能
    addDeco<ConsumptionMeasurer>(); //功耗测量功能
}
}
#endif
