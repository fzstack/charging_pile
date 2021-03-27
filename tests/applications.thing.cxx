#ifdef TEST_THING
#include <applications/thing_preset.hxx>
#include <map>
#include <string>
#include <functional>
#include <utilities/cmd.hxx>
#include <string>
#include <utilities/nat_port.hxx>
#include <utilities/f.hxx>
#include <utilities/json.hxx>
#include <components/persistent_storage_preset.hxx>
#include <components/state_store_preset.hxx>
#include <utilities/mp.hxx>

#define LOG_TAG "test.thing"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

//test_thing control [port] [timer_id] [minutes]
#if !defined(ENABLE_REMOTE) || (defined(ENABLE_REMOTE) && defined(UPPER_END))
void test_thing(int argc, char** argv) {
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
                thing->config(cmd.get<int>(), cmd.get<int>(), cmd.get<int>(), cmd.get<int>());
            }},
        });
    });
}
MSH_CMD_EXPORT(test_thing, );
#endif

int init_test_thing() {
    auto thing = Preset::Thing::get();

#if defined(ENABLE_REMOTE)
    Preset::Rpc::get();
#endif

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

    Preset::PersistentStorage::get();
    for(auto i = 0; i < Config::Bsp::kPortNum; i++) {
        magic_switch<Config::Bsp::kPortNum>{}([&](auto v){
            Preset::StateStore<decltype(v)::value>::get();
        }, i);
    }
#endif

    return RT_EOK;
}


INIT_APP_EXPORT(init_test_thing);
#endif
