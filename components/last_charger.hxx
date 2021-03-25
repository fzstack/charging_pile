#ifndef COMPONENTS_LAST_CHARGER_CXX_
#define COMPONENTS_LAST_CHARGER_CXX_

#include "state_store_base.hxx"
#include <config/bsp.hxx>
#include <array>
#include <memory>
#include <utilities/observable.hxx>
#include <utilities/observer.hxx>
#include <optional>

class LastCharger {
public:
    LastCharger();
    void watch(std::shared_ptr<StateStoreBase> store);

private:
    Observable<std::optional<std::shared_ptr<StateStoreBase>>> waitingToStart = {std::nullopt};
public:
    Observer<std::optional<std::shared_ptr<StateStoreBase>>> oWaitingToStart;

};

#include <utilities/singleton.hxx>
#include "state_store_preset.hxx"
#include <utilities/mp.hxx>
namespace Preset {
class LastCharger: public Singleton<LastCharger>, public ::LastCharger {
    friend class Singleton<LastCharger>;
    LastCharger(): ::LastCharger() {
        for(auto i = 0u; i < Config::Bsp::kPortNum; i++) {
            magic_switch<Config::Bsp::kPortNum>{}([&](auto x) {
                watch(StateStore<decltype(x)::value>::get());
            }, i);
        }
    }
};
}


#endif /* COMPONENTS_LAST_CHARGER_CXX_ */
