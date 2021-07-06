#ifndef COMPONENTS_CHARGER_HXX_
#define COMPONENTS_CHARGER_HXX_

#include "rgb_state_notifier.hxx"
#include <devices/relay.hxx>
#include "multimeter.hxx"
#include "virtual_load_detector.hxx"
#include "local_state_store.hxx"
#include <memory>

class Charger {
public:
    Charger(
        std::shared_ptr<Relay> relay,
        std::shared_ptr<Multimeter::Channel> multimeterChannel,
        std::shared_ptr<VirtualLoadDetector> vlodet,
        std::shared_ptr<StateStore> stateStore
    );
public:
    void init();
    void start();
    void stop();

private:
    void assertState(State::Value expectedState, std::string action);


private:
    std::shared_ptr<Relay> relay;

public:
    std::shared_ptr<Multimeter::Channel> multimeterChannel;

private:
    std::shared_ptr<VirtualLoadDetector> vlodet;

public:
    std::shared_ptr<StateStore> stateStore;

private:
    Observable<int> inited = {false};
};

struct charger_error: public std::runtime_error {
    using std::runtime_error::runtime_error;
};

#include <rtconfig.h>
#ifdef LOWER_END
#include <utilities/singleton.hxx>
#include "state_store_preset.hxx"
#include <utilities/mp.hxx>
namespace Preset {
template<int R>
class Charger: public Singleton<Charger<R>>, public ::Charger {
    friend class Singleton<Charger<R>>;
    Charger(): ::Charger(
        Relay<R>::get(),
        MultiMeterChannel<R>::get(),
        VirtualLoadDetector<R>::get(),
        StateStore<R>::get()
    ) {
        MultiMeterChannel<R>::Owner::get()->init();
        VirtualLoadDetector<R>::get()->init();
    }
};

class Chargers {
public:
    static std::array<std::shared_ptr<::Charger>, Config::Bsp::kPortNum> get() {
        auto r = decltype(get()){};
        for(auto i = 0; i < Config::Bsp::kPortNum; i++) {
            magic_switch<Config::Bsp::kPortNum>{}([&](auto v) {
                r[i] = Charger<decltype(v)::value>::get();
                r[i]->init();
            }, i);
        }
        return r;
    }
};

}
#endif

#endif /* COMPONENTS_CHARGER_HXX_ */
