extern "C" {
#include <fal.h>
}

#include <components/lower/lower_backup.hxx>
#include <rtthread.h>
#include <utilities/cmd.hxx>
#include <utilities/inner_port.hxx>
#include <utilities/nat_port.hxx>

static int init_lower_backup() {
    auto lowerBackup = Preset::LowerBackup::get();
    return RT_EOK;
}

static void lower_backup(int argc, char** argv) {
    auto lowerBackup = Preset::LowerBackup::get();
    Cmd{argc, argv}([&](Cmd& cmd){
        cmd.select({
            {"read", [&]{
                auto port = NatPort{cmd.get<uint8_t>()};
                rt_kprintf("port: %d\n", port.get());
                auto backup = lowerBackup->read(port);
                rt_kprintf("leftSeconds: %d\n", backup.leftSeconds);
                rt_kprintf("timerId: %d\n", backup.timerId);
                rt_kprintf("consumption: %d\n", (int)backup.consumption);
            }},
            {"write", [&]{
                auto backup = Backup{};
                auto port = NatPort{cmd.get<uint8_t>()};
                backup.leftSeconds = cmd.get<int>();
                backup.timerId = cmd.get<int>();
                backup.consumption = cmd.get<float>();
                rt_kprintf("writing...\n");
                rt_kprintf("leftSeconds: %d\n", backup.leftSeconds);
                rt_kprintf("timerId: %d\n", backup.timerId);
                rt_kprintf("consumption: %d\n", (int)backup.consumption);
                lowerBackup->write(port, backup);
            }},
            {"reset", [&]{
                lowerBackup->reset();
            }},
            {"erase", [&]{
                auto part = fal_partition_find(lowerBackup->kPartName);
                fal_partition_erase_all(part);
            }},
        });
    });
}

INIT_APP_EXPORT(init_lower_backup);
MSH_CMD_EXPORT(lower_backup,);