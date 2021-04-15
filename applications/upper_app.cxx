#include "upper_app.hxx"
#include <rtthread.h>
#include <utilities/f.hxx>

using namespace std;

UpperApp::UpperApp() {
    auto storage = Preset::PersistentStorage::get(); //~15.4k RAM
    auto ver = storage->read<Config::Version<>>();
    if(ver.updated == false) {
        ver.updated = true;
        storage->reset();
        rt_kprintf("old conf cleared\n");
        storage->write(ver);
    }
}

void UpperApp::run() {
    rt_kprintf("RUN APP\n");

    cloud->onControl += [=](auto port, auto timerId, auto minutes) {
        try {
            thing->control(port, timerId, minutes);
            return Cloud::ServiceResult::Succeed;
        } catch(const exception& e) {
            return Cloud::ServiceResult::Failed;
        }
    };
    cloud->onStop += [=](auto port, auto timerId) {
        try {
            thing->stop(port, timerId);
            return Cloud::ServiceResult::Succeed;
        } catch(const exception& e) {
            return Cloud::ServiceResult::Failed;
        }
    };
    cloud->onConfig += [=](auto conf) {
        thing->config(conf);
    };

    cloud->onQuery += [=]{
        thing->query();
    };

    cloud->onReboot += [=]{
        rebooter->rebootAll();
    };

    cloud->onReadConfig += [=] {
        return thing->readConfig();
    };

    cloud->onBroadcast += [=](auto balance, auto type) {
        user->boradcast(balance, type);
    };

    cloud->onOta += [=](std::string version, std::string module, std::shared_ptr<IStream> stream, int size) {
        ota->start(version, module, stream, size);
    };

    ota->onError += [=](auto module, auto e, auto desc) {
        cloud->emitOtaProgress((int)e, desc, module);
        state->progress = std::nullopt;
    };

    ota->onProgress += [=](auto module, auto value) {
        rt_kprintf("%d\n", value);
        cloud->emitOtaProgress(value, "download", module);
        state->progress = value;
    };

    ota->onDone += [=](auto module){
        rt_kprintf("OTA REBOOTING...\n");
        rebooter->reboot();
    };

    user->onInputConfirm += [=](auto port, auto icNumber) {
        cloud->emitIcNumber(port, icNumber);
    };

    thing->onPortAccess += [=](auto port) {
        cloud->emitPortAccess(port);
    };
    thing->onPortUnplug += [=](auto port) {
        if(rt_tick_get() < 1000) return;
        cloud->emitPortUnplug(port);
    };
    thing->onCurrentLimit += [=](auto port) {
        cloud->emitCurrentLimit(port);
    };
    thing->onCurrentData += [=](auto data){
        cloud->emitCurrentData(std::move(data));
    };
    state->cloudConnected.onChanged += [this](auto value) {
        watchDog->cancel();
        for(auto& modName: {"upper", "lower"}) {
            auto version = ota->getModule(modName)->getVersion();
            rt_kprintf("%s: %s\n", modName, version.c_str());
            cloud->emitModuleVersion(version, modName);
        }
    };
    watchDog->resetAfter(60);
    handshake->hello();
    thing->init();

    rt_kprintf("init cloud...\n");
    cloud->init();
}

#if defined(RUN_APP) && (!defined(ENABLE_REMOTE) || (defined(ENABLE_REMOTE) && defined(UPPER_END)))
void reset_config(int argc, char** argv) {
    auto storage = Preset::PersistentStorage::get();
    storage->reset();
}
MSH_CMD_EXPORT(reset_config, );
#endif

