#include <devices/relay.hxx>
#include <utilities/cmd.hxx>
#include <utilities/mp.hxx>
#include <map>
#include <string>
#include <utilities/nat_port.hxx>
#include <utilities/inner_port.hxx>

using namespace std;

void test_relay(int argc, char** argv) {
    //test_relay R [on|off]

    Cmd{argc, argv}([](Cmd& cmd){
        auto r = InnerPort{NatPort{cmd.get<rt_uint8_t>()}};
        auto vals = map<string, Relay::Value> {
            {"on", Relay::On},
            {"off", Relay::Off},
        };
        auto found = vals.find(cmd.get<string>());

        auto relay = std::shared_ptr<Relay>{};
        magic_switch<Config::Bsp::kPortNum>{}([&](auto v){
            relay = Preset::Relay<decltype(v)::value>::get();
        }, r.get());

        relay->value = found->second;
    });


    
}

MSH_CMD_EXPORT(test_relay, );
