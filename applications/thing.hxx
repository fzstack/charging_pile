#ifndef APPLICATIONS_THING_HXX_
#define APPLICATIONS_THING_HXX_

#include <components/charger.hxx>
#include <memory>
#include <stdexcept>
#include <components/timer.hxx>
#include "things/decos/base.hxx"
#include <Mutex.h>
#include "thing_base.hxx"

namespace Things::Decos {
class Base;
}

struct ChargerInfo {
    std::shared_ptr<Charger> charger = {};
    int timerId = 0;
    int leftSeconds = 0;
    float consumption = 0; //AxV
};

class Thing: public ThingBase {
public:
    friend class Things::Decos::Base;
    Thing(std::array<std::shared_ptr<Charger>, Config::Bsp::kPortNum> chargers);
    virtual void init() override;

    template<class T>
    auto addDeco() -> std::enable_if_t<std::is_base_of_v<Things::Decos::Base, T>> {
        auto deco = std::shared_ptr<T>(new T(this));
        decos.push_back(deco);
    }
    virtual void query() override;
    virtual void control(InnerPort port, int timerId, int minutes) override;
    virtual void stop(InnerPort port, int timerId) override;
    virtual void config(DevConfig conf) override;
    virtual DevConfig readConfig() override;

private:
    std::array<ChargerInfo, Config::Bsp::kPortNum> infos;
    std::list<std::shared_ptr<Things::Decos::Base>> decos = {};
    rtthread::Mutex mutex = {kMutex};

    Observable<bool> inited = {false};

    static const char* kMutex;
};

struct thing_error: public std::runtime_error {
    using std::runtime_error::runtime_error;
};

#include <utilities/singleton.hxx>
namespace Preset {
class ThingPre: public Singleton<ThingPre>, public ::Thing {
    friend class Singleton<ThingPre>;
    ThingPre();
};
}

#endif /* APPLICATIONS_THING_HXX_ */
