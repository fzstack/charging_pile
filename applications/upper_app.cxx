#include "upper_app.hxx"
#include <rtthread.h>
#include <utilities/f.hxx>

#include <config/co.hxx>
#include <components/packet.hxx>

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

    // cloud->onReboot += [=]{
    //     rebooter->rebootAll();
    // };

    cloud->onReadConfig += [=] {
        return thing->readConfig();
    };

    cloud->onBroadcast += [=](auto balance, auto type) {
        user->boradcast(balance, type);
    };

    cloud->onDebug += [=](auto type) {
        debugger->debug(type);
    };

    cloud->onOta += [=](std::string version, std::string module, std::string url, int size) {
        loader->ota(move(module), move(version), move(url), size);
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
        state->setCurrent(data.port, data.current);
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
    rt_kprintf("handshake OK!\n");
    thing->init();

    rt_kprintf("init cloud...\n");
    cloud->init();
}

