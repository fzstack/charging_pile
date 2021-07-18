#include "ali_cloud.hxx"
#include <components/air_components.hxx>
#include <config/app.hxx>
#include <utilities/json.hxx>
#include <components/air_components.hxx>

using namespace std;
using namespace json_literals;
using namespace string_literals;

AliCloud::AliCloud(std::shared_ptr<AliIotDevice> device, std::shared_ptr<Air724> air, std::shared_ptr<CloudTimer> timer, std::shared_ptr<AppState> appState): Cloud(timer), device(device), air(air), appState(appState) {
    // psTimer.onRun += [this, device] {
    //     auto port = InnerPort{rt_uint8_t(psCnt / kPsActionsNum)};
    //     auto& spec = specs[port.get()];
    //     switch(psCnt % kPsActionsNum) {
    //     case 0:
    //         if(spec.fPlugged.updateAndCheck()) {
    //             device->thread->exec([=]{
    //                 device->emit("port_access", {
    //                     {"port", NatPort{port}.get()},
    //                 });
    //             });
    //         }
    //         break;
    //     case 1:
    //         if(spec.fUnpluged.updateAndCheck()) {
    //             device->thread->exec([=]{
    //                 device->emit("port_unplug", {
    //                     {"port", NatPort{port}.get()},
    //                 });
    //             });
    //         }
    //         break;
    //     default:
    //         break;
    //     }
    //     psCnt++;
    //     psCnt %= Config::Bsp::kPortNum * kPsActionsNum;

    // };

    //可观测值绑定
    this->appState->cloudConnected = device->oConnected;

    inited.onChanged += [this](auto value) {
        if(!value) return;

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
#ifdef __cpp_exceptions
            try {
#endif
                onControl([r](auto result) mutable {
                    if(auto state = get_if<ServiceResult::Value>(&result)) {
                        r(Json {
                           {"state", int(*state)},
                        });
                    } else if(auto err = get_if<exception_ptr>(&result)) {
                        r(*err);
                    }
                }, NatPort{rt_uint8_t(params["port"_i])}, params["timer_id"], params["minutes"]);
#ifdef __cpp_exceptions
            } catch(const exception& e) {
                r(std::current_exception());
            }
#endif
        };

        this->device->services["stop"] += [this](auto r, const auto params)  {
#ifdef __cpp_exceptions
            try {
#endif
                onStop([r](auto result) mutable {
                    if(auto state = get_if<ServiceResult::Value>(&result)) {
                        r(Json {
                           {"state", int(*state)},
                        });
                    } else if(auto err = get_if<exception_ptr>(&result)) {
                        r(*err);
                    }
                }, NatPort{rt_uint8_t(params["port"_i])}, params["timer_id"]);
#ifdef __cpp_exceptions
            } catch(const exception& e) {
                r(std::current_exception());
            }
#endif
        };

        this->device->services["config"] += [this](auto r, const auto params)  {
#ifdef __cpp_exceptions
            try {
#endif
                onConfig([r](auto err) mutable {
                    if(!err) {
                        r(Json {});
                    } else {
                        r(*err);
                    }
                }, DevConfig{params["current_limit"], params["upload_thr"], params["fused_thr"], params["noload_curr_thr"], params["done_curr_thr"]});
#ifdef __cpp_exceptions
            } catch(const exception& e) {
                r(std::current_exception());
            }
#endif
        };

        this->device->services["rd_conf"] += [this](auto r, const auto params)  {
#ifdef __cpp_exceptions
            try {
#endif
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
#ifdef __cpp_exceptions
            } catch(const exception& e) {
                r(std::current_exception());
            }
#endif
        };

        this->device->services["broadcast"] += [this](auto r, const auto params)  {
#ifdef __cpp_exceptions
            try {
#endif
                onBroadcast([r](auto e) mutable {
                    if(e) {
                        r(*e);
                    } else {
                        r(Json {});
                    }
                }, params["balance"_i], (BroadcastType)params["type"_i]);
#ifdef __cpp_exceptions
            } catch(const exception& e) {
                r(std::current_exception());
            }
#endif
        };

        rt_kprintf("register ota callback\n");
        this->device->ota += [this](std::string version, std::string module, std::string url, int size) {
            rt_kprintf("ali cloud ota cb called\n");
            onOta(move(version), move(module), move(url), size);
        };

        this->air->init();
        string imei;
        {
            auto ess = this->air->make<AirEssential>();
            imei = ess->getImei();
            rt_kprintf("imei: %s\n", imei.data());
        }
        

        this->device->login(imei, Config::Cloud::productKey, Config::Cloud::productSecret);

        // psTimer.start();
        Cloud::init();
    };
}

void AliCloud::init() {
    inited = true;
}

void AliCloud::emitCurrentData(CurrentData&& data) {
    if(!device->oConnected) return;
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
    device->thread->exec([this, value]{
        rt_kprintf("try emit data\n");
        device->emit("current_data", value);
        rt_uint32_t used;
        rt_memory_info(nullptr, &used, nullptr);
        rt_kprintf("data emit succeed, used: %d\n", used);
    });
}

void AliCloud::setIccid(std::string_view iccid) {
    if(!device->oConnected) return;
    device->thread->exec([this, iccid = string{iccid}]{
        device->set("iccid", iccid);
    });
}

void AliCloud::emitHeartbeat(Heartbeat&& heartbeat) {
    if(!device->oConnected) return;
    device->thread->exec([=]{
        device->emit("heartbeat", {
            {"signal", heartbeat.signal},
            {"imei_suff", heartbeat.imeiSuff},
            {"temperature", heartbeat.temperature},
            {"humidity", heartbeat.humidity},
            {"smoke", heartbeat.smoke},
            {"timestamp", heartbeat.timestamp},
        });
    });
}

void AliCloud::emitPortAccess(NatPort port) {
    if(!device->oConnected) return;
    specs[InnerPort{port}.get()].fPlugged.retrigger();
}

void AliCloud::emitPortUnplug(NatPort port) {
    if(!device->oConnected) return;
    specs[InnerPort{port}.get()].fUnpluged.retrigger();
}

void AliCloud::emitIcNumber(NatPort port, std::string_view icNumber) {
    if(!device->oConnected) return;
    auto icNumberS = string{icNumber.begin(), icNumber.end()};
    device->thread->exec([=]{
        device->emit("ic_number", {
           {"port", port.get()},
           {"ic_number", icNumberS},
        });
    });
}

void AliCloud::emitCurrentLimit(NatPort port) {
    if(!device->oConnected) return;
    device->thread->exec([=]{
        device->emit("current_limit", {
           {"port", port.get()},
        });
    });
}

void AliCloud::emitModuleVersion(std::string_view version, std::string_view module) {
    if(!device->oConnected) return;
    device->thread->exec([this, version = string{version}, module = string{module}]{
        device->otaEmitVersion(version, module);
    });
}

void AliCloud::emitOtaProgress(int step, std::string_view desc, std::string_view module) {
    if(!device->oConnected) return;
    device->thread->exec([this, step, desc = string{desc}, module = string{module}]{
        device->otaEmitProgress(step, desc, module);
    });
}

