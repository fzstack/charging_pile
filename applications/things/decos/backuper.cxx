#include <utilities/mp.hxx>
#include "backuper.hxx"
#include <utilities/shared_thread.hxx>
#include <utilities/nat_port.hxx>
#include <utilities/inner_port.hxx>

using namespace std;
using namespace Things::Decos;

Backuper::Backuper(outer_t* outer): Base(outer) {
    timer.onRun += [this]{ //每10秒保存一次端口的剩余时间
        {
            auto port = InnerPort{currResumePort};
            currResumePort++;
            currResumePort %= Config::Bsp::kPortNum;
            auto& spec = specs[port.get()];
            auto guard = getLock();
            if(spec.fResume.updateAndCheck()) {
                resume(port);
            }
        }

        {
            auto guard = getLock();
            if(nextCount.updateAndCheck()) {
                nextCount.retrigger();
                auto ingore = false;
                auto nextBackupPort = currBackupPort;
                for(auto i = 0; i < Config::Bsp::kPortNum; i++) {
                    auto handlingPort = rt_uint8_t((currBackupPort + i) % Config::Bsp::kPortNum);
                    auto port = InnerPort{handlingPort};
                    auto& info = getInfo(port);
                    auto charger = info.charger;
                    auto& spec = specs[port.get()];
                    if(spec.backupCount.updateAndCheck()) {
                        if(!ingore) {
                            ingore = true;
                            nextBackupPort = (handlingPort + 1) % Config::Bsp::kPortNum;
                            if(charger->stateStore->oState.value() == State::Charging) {
                                spec.backupCount.retrigger();
                            }
                            auto backup = Backup {
                                leftSeconds: info.leftSeconds,
                                timerId: info.timerId,
                                consumption: info.consumption,
                            };
                            man->write(port, backup);
                            rt_kprintf("backup port%d, {ls: %d, it: %d, cs: %d}\n", NatPort{port}.get(), info.leftSeconds, info.timerId, (int)info.consumption);
                        } else  {
                            spec.backupCount.forceTrigger();
                        }
                    }
                }
                currBackupPort = nextBackupPort;
            }
        }

    };
}

void Backuper::init() {
    nextCount.retrigger();
    timer.start();
}

void Backuper::onStateChanged(InnerPort port, State::Value state) {
    auto& info = getInfo(port);
    auto& spec = specs[port.get()];
    auto charger = info.charger;
    auto guard = getLock();
    if(!spec.stateHasTransitioned) { //首次进行状态转移
        spec.fResume.trigger();
    } else {
        if(state == State::Charging || state == State::LoadWaitRemove) {
            rt_kprintf("backup port%d (state transition)\n", NatPort{port}.get());
            spec.backupCount.forceTrigger();
        }
    }
}

//do not use lock inside this func
void Backuper::resume(InnerPort port) {
    auto& info = getInfo(port);
    auto& spec = specs[port.get()];
    if(spec.stateHasTransitioned) return;
    auto charger = info.charger;
    auto state = charger->stateStore->oState.value().value_or(State::Error);
    auto backup = man->read(port);

    auto guard = getLock();
    spec.stateHasTransitioned = true;
    info.timerId = backup.timerId;
    if(backup.leftSeconds != 0) {
        info.leftSeconds = backup.leftSeconds;
        info.consumption = backup.consumption;
        rt_kprintf("resumed port%d, {ls: %d, it: %d, cs: %d}\n", NatPort{port}.get(), info.leftSeconds, info.timerId, (int)info.consumption);
        charger->start();
    } else {
        info.leftSeconds = 0;
        info.consumption = 0;
        spec.backupCount.trigger();
    }
}

