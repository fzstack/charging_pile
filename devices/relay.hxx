#ifndef DEVICES_RELAY_HXX_
#define DEVICES_RELAY_HXX_

#include <rtthread.h>
#include <rtdevice.h>
#include <devices/output_pin.hxx>
#include <utilities/observable.hxx>
#include <optional>

class Relay {
public:

    enum class Polar {
        High, Low,
    };

    Relay(rt_base_t pin, Polar polar);

    enum Value {
        On, Off,
    };

public:
    Observable<std::optional<Value>> value = {};

private:
    OutputPin pin;
    const Polar polar;
    const bool onPinVal = polar == Polar::High ? true : false;
    const bool offPinVal = !onPinVal;
};


#include <utilities/singleton.hxx>
#include <config/bsp.hxx>
namespace Preset {
template<int R>
class Relay: public Singleton<Relay<R>>, public Config::Bsp::assert_t<::Relay, R> {
    friend class Singleton<Relay<R>>;
    Relay(): ::Relay(kPin, ::Relay::Polar::Low) {}
    static const rt_base_t kPin;
};
}

#endif
