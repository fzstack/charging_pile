#include <utilities/mp.hxx>
#include <components/persistent_storage_preset.hxx>
#include "backuper.hxx"

using namespace std;
using namespace Things::Decos;

Backuper::Backuper(outer_t* outer): Base(outer) {
    inited.onChanged += [this](auto value) {
        if(!value) return;

        for(auto& spec: specs) {
            spec.timer = make_shared<Timer>(kDuration, kTimer);
        }

        for(rt_uint8_t i = 0u; i < Config::Bsp::kPortNum; i++) {
            auto& info = getInfo(InnerPort{rt_uint8_t(i)});
            auto& spec = specs[i];
            auto charger = info.charger;
            auto timer = spec.timer;
            charger->stateStore->oState += [this, charger, timer, i, &spec, &info](auto value) {
                if(!value) return;
                auto storage = Preset::PersistentStorage::get();
                if(!spec.stateHasTransitioned) { //首次进行状态转移
                    spec.stateHasTransitioned = true;
                    magic_switch<Config::Bsp::kPortNum>{}([&](auto x){
                        auto backup = storage->make<Backup<decltype(x)::value>>();
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
                    }, i);
                } else {
                    magic_switch<Config::Bsp::kPortNum>{}([&](auto x){
                        auto backup = storage->make<Backup<decltype(x)::value>>();
                        switch(*value) {
                        case State::Charging:
                            timer->start();
                            break;
                        case State::LoadWaitRemove:
                            timer->stop();
                            break;
                        default:
                            break;
                        }
                        if(value == State::Charging || value == State::LoadWaitRemove) {
                            rt_kprintf("backup port%d due to state transition, {left: %d}\n", i, info.leftSeconds);
                            backup->leftSeconds = info.leftSeconds;
                            backup->timerId = info.timerId;
                            backup->consumption = info.consumption;
                        }
                    }, i);
                }
            };

            timer->onRun += [this, i]{ //每10秒保存一次端口的剩余时间
                auto storage = Preset::PersistentStorage::get();
                magic_switch<Config::Bsp::kPortNum>{}([&](auto x){
                    auto backup = storage->make<Backup<decltype(x)::value>>();
                    auto& info = getInfo(InnerPort{i});
                    rt_kprintf("backup port%d due to timing, {left: %d}\n", i, info.leftSeconds);
                    backup->leftSeconds = info.leftSeconds;
                    backup->consumption = info.consumption;
                }, i);
            };
        }
    };
}

void Backuper::init() {
    inited = true;
}

