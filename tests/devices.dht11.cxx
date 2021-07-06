#include <devices/dht11.hxx>
#include <utilities/cmd.hxx>
#include <string>

using namespace std;

void test_dht11(int argc, char** argv) {
    Cmd{argc, argv}([](Cmd& cmd){
        cmd.select<string>({
            {"get", [&]{
                auto dht11 = Preset::Dht11::get();
                cmd.select<string>({
                    {"humi", [&]{
                        rt_kprintf("humi: %d%%\n", int(dht11->oHumidity.value()));
                    }},
                    {"temp", [&]{
                        rt_kprintf("temp: %d℃\n", int(dht11->oTemperature.value()));
                    }}
                });
            }}
        });
    });


}

MSH_CMD_EXPORT(test_dht11, );

int init_test_dht11() {
    Preset::Dht11::get();
    rt_kprintf("dht11 init ok\n");
    return RT_EOK;
}
INIT_APP_EXPORT(init_test_dht11);
