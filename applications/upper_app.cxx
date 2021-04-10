#include "upper_app.hxx"
#include <rtthread.h>
#include <utilities/mp.hxx>

using namespace std;

UpperApp::UpperApp() {
    Preset::PersistentStorage::get(); //~15.4k RAM
}

void UpperApp::run() {
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
        rebooter->reboot();
    };

    cloud->onReadConfig += [=] {
        return thing->readConfig();
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
    handshake->hello();
    thing->init();
    cloud->init();
}

#if defined(RUN_APP) && (!defined(ENABLE_REMOTE) || (defined(ENABLE_REMOTE) && defined(UPPER_END)))
void reset_config(int argc, char** argv) {
    auto storage = Preset::PersistentStorage::get();
    storage->reset();
}
MSH_CMD_EXPORT(reset_config, );
#endif

