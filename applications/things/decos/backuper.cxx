#include <utilities/mp.hxx>
#include <components/persistent_storage_preset.hxx>
#include "backuper.hxx"
#include <utilities/shared_thread.hxx>
#include <utilities/nat_port.hxx>
#include <utilities/inner_port.hxx>

using namespace std;
using namespace Things::Decos;

Backuper::Backuper(outer_t* outer): Base(outer) {
    timer.onRun += [this]{ //每10秒保存一次端口的剩余时间
        auto port = InnerPort{currPort};
        currPort++;
        currPort %= Config::Bsp::kPortNum;
        auto storage = Preset::PersistentStorage::get();
        auto& info = getInfo(port);
        auto& spec = specs[port.get()];
        auto charger = info.charger;
        auto guard = getLock();
        if(spec.fResume.updateAndCheck()) {
            resume(port);
        }
        if(spec.count.updateAndCheck()) {
            if(charger->stateStore->oState.value() == State::Charging) {
                spec.count.retrigger();
            }
            magic_switch<Config::Bsp::kPortNum>{}([&](auto x){
                storage->write(Backup<decltype(x)::value> {
                    leftSeconds: info.leftSeconds,
                    timerId: info.timerId,
                    consumption: info.consumption,
                });
                rt_kprintf("backup port%d, {left: %d}\n", NatPort{port}.get(), info.leftSeconds);
            }, port.get());
        }
    };
}

void Backuper::init() {
    timer.start();
}

void Backuper::onStateChanged(InnerPort port, State::Value state) {
    auto storage = Preset::PersistentStorage::get();
    auto& info = getInfo(port);
    auto& spec = specs[port.get()];
    auto charger = info.charger;
    auto guard = getLock();
    magic_switch<Config::Bsp::kPortNum>{}([&](auto x){
        if(!spec.stateHasTransitioned) { //首次进行状态转移
            spec.fResume.trigger();
        } else {
            if(state == State::Charging || state == State::LoadWaitRemove) {
                rt_kprintf("backup port%d (state transition)\n", NatPort{port}.get());
                spec.count.forceTrigger();
            }
        }
    }, port.get());
}

//do not use lock inside this func
void Backuper::resume(InnerPort port) {
    auto storage = Preset::PersistentStorage::get();
    auto& info = getInfo(port);
    auto& spec = specs[port.get()];
    if(spec.stateHasTransitioned) return;
    auto charger = info.charger;
    auto state = charger->stateStore->oState.value().value_or(State::Error);
    magic_switch<Config::Bsp::kPortNum>{}([&](auto x){
        storage->read<Backup<decltype(x)::value>>([this, charger, &info, &spec, port, state](auto backup) mutable {
            if(backup == nullopt) {
                rt_kprintf("[%d]retry res\n", NatPort{port}.get());
                spec.fResume.retrigger();
                return;
            }
            auto guard = getLock();
            spec.stateHasTransitioned = true;
            info.timerId = backup->timerId;
            if(state == State::LoadInserted && backup->leftSeconds != 0) {
                rt_kprintf("port%d state resumed\n", NatPort{port}.get());
                info.leftSeconds = backup->leftSeconds;
                info.consumption = backup->consumption;
                charger->start();
            } else {
                info.leftSeconds = 0;
                info.consumption = 0;
                spec.count.trigger();
            }
        });
    }, port.get());
}

