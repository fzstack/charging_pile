#include "upper_app.hxx"
#include <rtthread.h>
#include <utilities/f.hxx>

using namespace std;

UpperApp::UpperApp() {
    auto storage = Preset::PersistentStorage::get(); //~15.4k RAM
    auto ver = storage->read<Config::Version<4>>();
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

    cloud->onHeartbeat += [=] {
        if(!state->cloudConnected) return;
        if(ota->isRunning()) return;
        cloud->emitHeartbeat(std::move(Heartbeat {
            signal: state->signal,
            imeiSuff: state->imei,
            temperature: (int)state->dht11->oTemperature,
            humidity: (int)state->dht11->oHumidity,
            smoke: 0,
            timestamp: (int)rt_tick_get(),
        }));
    };

    ota->onError += [=](auto module, auto e, auto desc) {
        cloud->emitOtaProgress((int)e, desc, module);
        state->progress = std::nullopt;
    };

    ota->onProgress += [=](auto module, auto value) {
        // cloud->emitOtaProgress(value, "download", module);
        state->progress = value;
    };

    state->progress.onChanged += [=](auto value) {
        if(value == std::nullopt) return;
        rt_kprintf("%d\n", *value);
    };

    ota->onDone += [=](auto module){
        rt_kprintf("OTA REBOOTING...\n");
        rebooter->rebootModule(module);
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
        //watchDog->cancel();
        cloud->setIccid(state->iccid);
        for(auto& modName: {"upper", "lower"}) {
            ota->getModule(modName)->getVersion([this, modName](auto v) {
                auto version = std::get_if<std::string>(&v);
                if(version != nullptr) {
                    rt_kprintf("%s: %s\n", modName, version->c_str());
                    cloud->emitModuleVersion(*version, modName);
                }
            });
        }
    };
    //watchDog->resetAfter(60);
    handshake->hello();
    thing->init();

    rt_kprintf("init cloud...\n");
    cloud->init();
}

