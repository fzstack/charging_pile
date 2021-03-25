#ifdef TEST_SPI_HLW8112

#include <rtthread.h>
#include <devices/spi_hlw8112.hxx>
#include <utilities/cmd.hxx>
#include <utilities/mp.hxx>

#define LOG_TAG "test.spi_hlw"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;
using namespace Hlw8112ns::Regs;

static void test_spi_hlw8112(int argc, char** argv) {
    ASSERT_MIN_NARGS(2);
    auto r = atoi(argv[1]);
    ASSERT_ARG(r, 0 <= r && r < Preset::Trait<SpiHlw8112>::Num);

    auto hlw8112 = shared_ptr<SpiHlw8112>{};

    magic_switch<Preset::Trait<SpiHlw8112>::Num>{}([&](auto v){
        hlw8112 = Preset::SpiHlw8112<decltype(v)::value>::get();
    }, r);

    auto emucon2Sess = hlw8112->makeSess<emucon2>();
    rt_kprintf("emucon2: %04x\n", *(rt_uint16_t*)&*emucon2Sess);

    auto emuconSess = hlw8112->makeSess<emucon>();
    rt_kprintf("emucon: %04x\n", *(rt_uint16_t*)&*emuconSess);

    auto pinSess = hlw8112->makeSess<pin>();
    rt_kprintf("pin: %04x\n", *(rt_uint16_t*)&*pinSess);

    auto ieSess = hlw8112->makeSess<ie>();
    rt_kprintf("ie: %04x\n", *(rt_uint16_t*)&*ieSess);

    auto sysconSess = hlw8112->makeSess<syscon>();
    rt_kprintf("syscon: %04x\n", *(rt_uint16_t*)&*sysconSess);
}

static int init_test_spi_hlw8112() {
    for(auto i = 0; i < Preset::Trait<SpiHlw8112>::Num; i++) {
        magic_switch<Preset::Trait<SpiHlw8112>::Num>{}([](auto v) {
            Preset::SpiHlw8112<decltype(v)::value>::get()->init();
        }, i);
    }
    return RT_EOK;
}

MSH_CMD_EXPORT(test_spi_hlw8112, );
INIT_APP_EXPORT(init_test_spi_hlw8112);

#endif

