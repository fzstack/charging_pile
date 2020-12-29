#ifndef DEVICES_RELAY_HXX_
#define DEVICES_RELAY_HXX_

#include <rtthread.h>
#include <rtdevice.h>
#include <devices/output_pin.hxx>

using Relay = OutputPin;

#include <utilities/singleton.hxx>
#include <config/bsp.hxx>
namespace Preset {
template<int R>
class Relay: public Singleton<Relay<R>>, public Config::Bsp::assert_t<::Relay, R> {
    friend class Singleton<Relay<R>>;
    Relay(): ::Relay(kPin) {}
    static const rt_base_t kPin;
};
}

#endif
