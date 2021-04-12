#include "ali_cloud.hxx"
#include <components/air_components.hxx>
#include <config/app.hxx>
#include <utilities/json.hxx>

using namespace std;
using namespace json_literals;
using namespace string_literals;

AliCloud::AliCloud(std::shared_ptr<AliIotDevice> device, std::shared_ptr<Air724> air, std::shared_ptr<CloudTimer> timer, std::shared_ptr<AppState> appState): Cloud(timer), device(device), air(air), appState(appState) {
    psTimer.onRun += [this, device] {
        auto port = InnerPort{rt_uint8_t(psCnt / kPsActionsNum)};
        auto& spec = specs[port.get()];
        switch(psCnt % kPsActionsNum) {
        case 0:
            if(spec.fPlugged.updateAndCheck()) {
                runOn(device->thread->post([=] () {
                    device->emit("port_access", {
                        {"port", NatPort{port}.get()},
                    });
                }));
            }
            break;
        case 1:
            if(spec.fUnpluged.updateAndCheck()) {
                runOn(device->thread->post([=] () {
                    device->emit("port_unplug", {
                        {"port", NatPort{port}.get()},
                    });
                }));
            }
            break;
        default:
            break;
        }


        psCnt++;
        psCnt %= Config::Bsp::kPortNum * kPsActionsNum;

    };

    inited.onChanged += [this](auto value) {
        if(!value) return;

        rt_kprintf("initing ali cloud\n");
        this->air->init();
        rt_kprintf("air init OK\n");
        this->device->services["query"] += [this](auto r, const auto params) {
          onQuery([r](auto e) mutable {
              if(e) {
                  r(*e);
              } else {
                  r(Json {});
              }
          });
        };

        this->device->services["reboot"] += [this](auto r, const auto params) {
          onReboot([r](auto e) mutable {
              if(e) {
                  r(*e);
              } else {
                  r(Json {});
              }
          });
        };

        this->device->services["control"] += [this](auto r, const auto params) {
            try {
                onControl([r](auto result) mutable {
                    if(auto state = get_if<ServiceResult::Value>(&result)) {
                        r(Json {
                           {"state", int(*state)},
                        });
                    } else if(auto err = get_if<exception_ptr>(&result)) {
                        r(*err);
                    }
                }, NatPort{rt_uint8_t(params["port"_i])}, params["timer_id"], params["minutes"]);
            } catch(const exception& e) {
                r(std::current_exception());
            }
        };

        this->device->services["stop"] += [this](auto r, const auto params)  {
            try {
                onStop([r](auto result) mutable {
                    if(auto state = get_if<ServiceResult::Value>(&result)) {
                        r(Json {
                           {"state", int(*state)},
                        });
                    } else if(auto err = get_if<exception_ptr>(&result)) {
                        r(*err);
                    }
                }, NatPort{rt_uint8_t(params["port"_i])}, params["timer_id"]);
            } catch(const exception& e) {
                r(std::current_exception());
            }
        };

        this->device->services["config"] += [this](auto r, const auto params)  {
            try {
                onConfig([r](auto err) mutable {
                    if(!err) {
                        r(Json {});
                    } else {
                        r(*err);
                    }
                }, DevConfig{params["current_limit"], params["upload_thr"], params["fused_thr"], params["noload_curr_thr"], params["done_curr_thr"]});
            } catch(const exception& e) {
                r(std::current_exception());
            }
        };

        this->device->services["rd_conf"] += [this](auto r, const auto params)  {
            try {
                onReadConfig([r](auto result) mutable {
                    if(auto conf = get_if<DevConfig>(&result)) {
                        r(Json {
                           {"current_limit", conf->currentLimit},
                           {"upload_thr", conf->uploadThr},
                           {"fused_thr", conf->fuzedThr},
                           {"noload_curr_thr", conf->noloadCurrThr},
                           {"done_curr_thr", conf->doneCurrThr},
                        });
                    } else if(auto err = get_if<exception_ptr>(&result)) {
                        r(*err);
                    }
                });
            } catch(const exception& e) {
                r(std::current_exception());
            }
        };

        this->device->services["broadcast"] += [this](auto r, const auto params)  {
            try {
                onBroadcast([r](auto e) mutable {
                    if(e) {
                        r(*e);
                    } else {
                        r(Json {});
                    }
                }, params["balance"_i], (BroadcastType)params["type"_i]);
            } catch(const exception& e) {
                r(std::current_exception());
            }
        };

        rt_kprintf("register ota callback\n");
        this->device->ota += [this](std::string version, std::string module, std::shared_ptr<IStream> stream, int size) {
            rt_kprintf("ali cloud ota cb called\n");
            onOta(version, module, stream, size);
        };

        rt_kprintf("try login device...\n");
        this->device->login(this->appState->imei, Config::Cloud::productKey, Config::Cloud::productSecret);

        runOn(this->device->thread->post([=]{
            auto imei = this->appState->imei;
            imei = imei.substr(imei.size() - 12);
            heartbeat += this->device->thread->post([=](){
                if(!firstBeated) {
                    firstBeated = true;
                    setIccid(this->appState->iccid);
                    emitModuleVersion(Version::upper, "default");
                }
                auto hb = Heartbeat {
                    signal: *this->appState->signal,
                    imeiSuff: imei,
                    temperature: (int)this->appState->dht11->oTemperature,
                    humidity: (int)this->appState->dht11->oHumidity,
                    smoke: 0,
                    timestamp: (int)rt_tick_get(),
                };
                emitHeartbeat(std::move(hb));

                rt_uint32_t total, maxUsed;
                rt_memory_info(&total, nullptr, &maxUsed);
                this->device->log("mem: "s + to_string(100 * maxUsed / total) + "%, middle: " + to_string(this->device->thread->maxUsed()) + "%");
                onTimer();
            });
        }));

        psTimer.start();
        Cloud::init();
        initDone = true;
        this->appState->cloudConnected = true;
    };
}

void AliCloud::init() {
    inited = true;
}

void AliCloud::emitCurrentData(CurrentData&& data) {
    if(!initDone) return;
    auto value = Json {
        {"port", data.port.get()},
        {"timer_id", data.timerId},
        {"left_minutes", data.leftMinutes},
        {"state", data.state},
        {"current", data.current},
        {"voltage", data.voltage},
        {"consumption", data.consumption},
        {"fuse", data.fuse},
    };
    runOn(device->thread->post([this, value]{
        rt_kprintf("try emit data\n");
        device->emit("current_data", value);
        rt_uint32_t used;
        rt_memory_info(nullptr, &used, nullptr);
        rt_kprintf("data emit succeed, used: %d\n", used);

    }));
}

void AliCloud::setIccid(std::string_view iccid) {
    if(!initDone) return;
    auto iccidStr = string{iccid};
    runOn(device->thread->post([=]{
        device->set("iccid", iccidStr);
    }));
}

void AliCloud::emitHeartbeat(Heartbeat&& heartbeat) {
    if(!initDone) return;
    runOn(device->thread->post([=]{
        device->emit("heartbeat", {
            {"signal", heartbeat.signal},
            {"imei_suff", heartbeat.imeiSuff},
            {"temperature", heartbeat.temperature},
            {"humidity", heartbeat.humidity},
            {"smoke", heartbeat.smoke},
            {"timestamp", heartbeat.timestamp},
        });
    }));
}

void AliCloud::emitPortAccess(NatPort port) {
    if(!initDone) return;
    specs[InnerPort{port}.get()].fPlugged.retrigger();
}

void AliCloud::emitPortUnplug(NatPort port) {
    if(!initDone) return;
    specs[InnerPort{port}.get()].fUnpluged.retrigger();
}

void AliCloud::emitIcNumber(NatPort port, std::string_view icNumber) {
    if(!initDone) return;
    auto icNumberS = string{icNumber.begin(), icNumber.end()};
    runOn(device->thread->post([=]{
        device->emit("ic_number", {
           {"port", port.get()},
           {"ic_number", icNumberS},
        });
    }));
}

void AliCloud::emitCurrentLimit(NatPort port) {
    if(!initDone) return;
    runOn(device->thread->post([=]{
        device->emit("current_limit", {
           {"port", port.get()},
        });
    }));
}

void AliCloud::emitModuleVersion(std::string_view version, std::string_view module) {
    if(!initDone) return;
    auto versionS = string{version.begin(), version.end()};
    auto moduleS = string{module.begin(), module.end()};
    runOn(device->thread->post([=]{
        device->otaEmitVersion(versionS, moduleS);
    }));
}

void AliCloud::emitOtaProgress(int step, std::string_view desc, std::string_view module) {
    if(!initDone) return;
    runOn(device->thread->post([this, step, desc = string{desc}, module = string{module}]{
        device->otaEmitProgress(step, desc, module);
    }));
}

void AliCloud::setSignalInterval() {
    heartbeat();
}

