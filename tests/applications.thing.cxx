#ifdef TEST_THING
#include <applications/thing_preset.hxx>
#include <map>
#include <string>
#include <functional>
#include <utilities/cmd.hxx>
#include <string>
#include <utilities/nat_port.hxx>

#define LOG_TAG "test.thing"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

//test_thing control [port] [timer_id] [minutes]
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

int init_test_thing() {
    auto thing = Preset::Thing::get();

    thing->onPortAccess += [](auto port) {
        LOG_I("port accessed: %d", port);
    };

    return RT_EOK;
}

MSH_CMD_EXPORT(test_thing, );
INIT_APP_EXPORT(init_test_thing);
#endif
