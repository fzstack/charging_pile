#include <utilities/mp.hxx>
#include <components/persistent_storage_preset.hxx>
#include "backuper.hxx"
#include <utilities/shared_thread.hxx>
#include <utilities/nat_port.hxx>
#include <utilities/inner_port.hxx>

using namespace std;
using namespace Things::Decos;

//#define LOG_PKG_DEAD

Backuper::Backuper(outer_t* outer): Base(outer) {
    inited.onChanged += [this](auto value) {
        if(!value) return;

        for(rt_uint8_t i = 0u; i < Config::Bsp::kPortNum; i++) {
            auto& info = getInfo(InnerPort{rt_uint8_t(i)});
            auto& spec = specs[i];
            auto charger = info.charger;
            charger->stateStore->oState += [this, charger, i, &spec, &info](auto value) {
                if(!value) return;
                auto storage = Preset::PersistentStorage::get();
                if(!spec.stateHasTransitioned) { //首次进行状态转移
                    spec.stateHasTransitioned = true;
                    magic_switch<Config::Bsp::kPortNum>{}([&](auto x){
#ifdef LOG_PKG_DEAD
                        rt_kprintf("curr thread: %s\n", rt_thread_self()->name);
#endif
                        storage->make<Backup<decltype(x)::value>>([this, value, charger, i, &info](auto backup){
                            switch(*value) {
                            case State::LoadInserted:
                                if(backup->leftSeconds != 0) {
                                    auto backupCopy = *backup;
                                    rt_kprintf("port%d state resumed\n", i);
                                    auto guard = getLock();
                                    charger->start();
                                    info.leftSeconds = backupCopy.leftSeconds;
                                    info.timerId = backupCopy.timerId;
                                    info.consumption = backupCopy.consumption;
                                }
                                break;
                            case State::LoadNotInsert:
                                backup->leftSeconds = 0;
                                backup->consumption = 0;
                                break;
                            default:
                                break;
                            }
                        });
                    }, i);
                } else {
                    magic_switch<Config::Bsp::kPortNum>{}([&](auto x){
                        if(value == State::Charging || value == State::LoadWaitRemove) {
#ifdef LOG_PKG_DEAD
                            rt_kprintf("curr thread: %s\n", rt_thread_self()->name);
#endif
                            rt_kprintf("backup port%d due to state transition, {left: %d}\n", NatPort{InnerPort{i}}.get(), info.leftSeconds);
                            storage->make<Backup<decltype(x)::value>>([this, &info](auto backup){
                                auto guard = getLock();
                                backup->leftSeconds = info.leftSeconds;
                                backup->timerId = info.timerId;
                                backup->consumption = info.consumption;
                            });
                        }
                    }, i);
                }
            };
        }

        timer.onRun += [this]{ //每10秒保存一次端口的剩余时间
            auto i = currPort;
            currPort++;
            currPort %= Config::Bsp::kPortNum;
            auto storage = Preset::PersistentStorage::get();
            auto charger = getInfo(InnerPort{rt_uint8_t(i)}).charger;
            if(charger->stateStore->oState.value() != State::Charging) return;
            magic_switch<Config::Bsp::kPortNum>{}([&](auto x){
                storage->make<Backup<decltype(x)::value>>([this, i, charger](auto backup){
                    auto& info = getInfo(InnerPort{i});
                    auto guard = getLock();
                    if(charger->stateStore->oState.value() != State::Charging) return;
                    rt_kprintf("backup port%d due to timing, {left: %d}\n", NatPort{InnerPort{i}}.get(), info.leftSeconds);
                    backup->leftSeconds = info.leftSeconds;
                    backup->consumption = info.consumption;
                });
            }, i);
        };

        timer.start();
    };
}

void Backuper::init() {
    inited = true;
}

