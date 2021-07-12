#include <applications/ota.hxx>
#include <rtthread.h>
#include <utilities/cmd.hxx>
#include <components/persistent_storage_preset.hxx>
#include <config/app.hxx>

static void ota(int argc, char** argv) {
    Cmd{argc, argv}([](Cmd& cmd) {
        cmd.select({
        {"offset", [&]{
            cmd.select({
            {"clear", [&]{
                auto storage = Preset::PersistentStorage::get();
                rt_kprintf("reading conf\n");
                auto conf = storage->read<Config::OTA>();
                conf.offsets = {};
                rt_kprintf("writing conf\n");
                storage->write(conf);
                rt_kprintf("done\n");
            }},
            });
        }},
        });
    });
}

MSH_CMD_EXPORT(ota, );


