#include <devices/relay.hxx>
#include <utilities/cmd.hxx>
#include <utilities/mp.hxx>
#include <map>
#include <string>
#include <utilities/nat_port.hxx>
#include <utilities/inner_port.hxx>

#define LOG_TAG "test.relay"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

void test_relay(int argc, char** argv) {
    //test_relay R [on|off]
    ASSERT_MIN_NARGS(3);

    auto r = InnerPort{NatPort{(rt_uint8_t)atoi(argv[1])}}.get();
    ASSERT_ARG(r, 0 <= r && r < Config::Bsp::kPortNum);

    auto vals = map<string, Relay::Value> {
        {"on", Relay::On},
        {"off", Relay::Off},
    };
    auto found = vals.find(argv[2]);
    ASSERT_ARG(action, found != vals.end());

    auto relay = std::shared_ptr<Relay>{};
    magic_switch<Config::Bsp::kPortNum>{}([&](auto v){
        relay = Preset::Relay<decltype(v)::value>::get();
    }, r);

    relay->value = found->second;
}

MSH_CMD_EXPORT(test_relay, );
