#include <applications/thing_preset.hxx>
#include <map>
#include <string>
#include <functional>
#include <utilities/cmd.hxx>
#include <string>
#include <utilities/nat_port.hxx>
#include <utilities/f.hxx>
#include <components/state_store_preset.hxx>
#include <utilities/mp.hxx>
#include <components/handshake.hxx>
#include <rtthread.h>

using namespace std;

// test_thing control [port] [timer_id] [minutes]
static void thing(int argc, char** argv) {
   Cmd{argc, argv}([](Cmd& cmd) {
       auto thing = Preset::Thing::get();
       cmd.select<string>({
           {"query", [&]{
               thing->query();
           }},
           {"control", [&]{
               thing->control(NatPort{cmd.get<rt_uint8_t>()}, cmd.get<int>(), cmd.get<int>());
           }},
           {"stop", [&]{
               thing->stop(NatPort{cmd.get<rt_uint8_t>()}, cmd.get<int>());
           }},
           {"config", [&]{
               thing->config({cmd.get<int>(), cmd.get<int>(), cmd.get<int>(), cmd.get<int>()});
           }},
       });
   });
}
MSH_CMD_EXPORT(thing, );

#if !defined(ENABLE_REMOTE) || (defined(ENABLE_REMOTE) && defined(UPPER_END))
void reset_config(int argc, char** argv) {
    auto storage = Preset::PersistentStorage::get();
    storage->reset();
}
MSH_CMD_EXPORT(reset_config, );
#endif

static int init_test_thing() {
    auto thing = Preset::Thing::get();
#if defined(ENABLE_REMOTE)
    // auto handshake = Preset::Handshake::get();
    // rt_kprintf("handshaking...\n");
    // handshake->hello();
    // rt_kprintf("hello!\n");
#endif
    thing->init();


#if !defined(ENABLE_REMOTE) || (defined(ENABLE_REMOTE) && defined(UPPER_END))
    thing->onPortAccess += [](NatPort port) {
        F{} << "port "_r << port.get() << " accessed"_r << endln;

    };

    thing->onPortUnplug += [](NatPort port) {
        F{} << "port "_r << port.get() << " unpluged"_r << endln;
    };

    thing->onCurrentLimit += [](NatPort port) {
        F{} << "port "_r << port.get() << " on current limit"_r << endln;
    };

    thing->onCurrentData += [](CurrentData data) {
        F{} << "current data: "_r << to_string(Json {
            {"port", data.port.get()},
            {"timerId", data.timerId},
            {"leftMinutes", data.leftMinutes},
            {"state", data.state},
            {"current", data.current},
            {"voltage", data.voltage},
            {"consumption", data.consumption},
            {"fuse", data.fuse},
        }) << endln;
    };

#endif

    return RT_EOK;
}


INIT_APP_EXPORT(init_test_thing);
