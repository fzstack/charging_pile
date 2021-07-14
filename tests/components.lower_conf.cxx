extern "C" {
#include <fal.h>
}

#include <components/lower/lower_conf.hxx>
#include <rtthread.h>
#include <utilities/cmd.hxx>

static int init_lower_conf() {
    auto lowerConf = Preset::LowerConf::get();
    return RT_EOK;
}

static void lower_conf(int argc, char** argv) {
    auto lowerConf = Preset::LowerConf::get();
    Cmd{argc, argv}([&](Cmd& cmd){
        cmd.select({
            {"read", [&]{
                auto conf = lowerConf->read();
                rt_kprintf("currentLimit: %d\n", conf.currentLimit);
                rt_kprintf("uploadThr: %d\n", conf.uploadThr);
                rt_kprintf("fuzedThr: %d\n", conf.fuzedThr);
                rt_kprintf("noloadCurrThr: %d\n", conf.noloadCurrThr);
                rt_kprintf("doneCurrThr: %d\n", conf.doneCurrThr);
            }},
            {"write", [&]{
                auto conf = DevConfig{};
                conf.currentLimit = cmd.get<int>();
                conf.uploadThr = cmd.get<int>();
                conf.fuzedThr = cmd.get<int>();
                conf.noloadCurrThr = cmd.get<int>();
                conf.doneCurrThr = cmd.get<int>();
                lowerConf->write(conf);
            }},
            {"reset", [&]{
                // lowerConf->reset();
            }},
            {"erase", [&]{
                auto part = fal_partition_find("conf");
                fal_partition_erase_all(part);
            }},
        });
    });
}

INIT_APP_EXPORT(init_lower_conf);
MSH_CMD_EXPORT(lower_conf,);