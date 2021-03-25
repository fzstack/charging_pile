#include "ali_cloud.hxx"
#include <components/air_components.hxx>
#include <config/app.hxx>
#include <utilities/json.hxx>

using namespace std;
using namespace json_literals;

AliCloud::AliCloud(std::shared_ptr<AliIotDevice> device, std::shared_ptr<Air724> air, std::shared_ptr<CloudTimer> timer): Cloud(timer), device(device), air(air) {
    inited.onChanged += [this](auto value) {
        if(!value) return;

        rt_kprintf("initing ali cloud\n");
        this->air->init();
        this->device->services["query"] += [this](auto r, const auto params) {
          onQuery([r](auto e) mutable {
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
                }, params["current_limit"], params["upload_thr"], params["fused_thr"], params["noload_curr_thr"]);
            } catch(const exception& e) {
                r(std::current_exception());
            }
        };

        this->device->login(Config::App::cloudDeviceName, Config::App::cloudProductKey, Config::App::cloudDeviceSecret);

        runOn(this->device->thread->post([=]{
            auto ess = this->air->make<AirEssential>();
            setIccid(ess->getIccid());
            auto imei = ess->getImei();
            imei = imei.substr(imei.size() - 12);
            heartbeat += this->device->thread->post([=](){
                auto ess = this->air->make<AirEssential>();
                auto hb = Heartbeat {
                    signal: ess->getCsq(),
                    imeiSuff: imei,
                    temperature: 0,
                    humidity: 0,
                    smoke: 0,
                    timestamp: 0,
                };
                emitHeartbeat(std::move(hb));
            });
        }));

        Cloud::init();
    };
}

void AliCloud::init() {
    inited = true;
}

void AliCloud::emitCurrentData(CurrentData&& data) {
    if(rt_tick_get() - lastSetTick < 2000)
        return;
    lastSetTick = rt_tick_get();
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
    auto iccidStr = string{iccid};
    runOn(device->thread->post([=]{
        device->set("iccid", iccidStr);
    }));
}

void AliCloud::emitHeartbeat(Heartbeat&& heartbeat) {
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
    runOn(device->thread->post([=]{
        device->emit("port_access", {
            {"port", port.get()},
        });
    }));
}

void AliCloud::emitPortUnplug(NatPort port) {
    runOn(device->thread->post([=]{
        device->emit("port_unplug", {
            {"port", port.get()},
        });
    }));
}

void AliCloud::emitIcNumber(NatPort port, std::string_view icNumber) {
    auto icNumberS = string{icNumber.begin(), icNumber.end()};
    runOn(device->thread->post([=]{
        device->emit("ic_number", {
           {"port", port.get()},
           {"ic_number", icNumberS},
        });
    }));
}

void AliCloud::emitCurrentLimit(NatPort port) {
    runOn(device->thread->post([=]{
        device->emit("current_limit", {
           {"port", port.get()},
        });
    }));
}

void AliCloud::setSignalInterval() {
    heartbeat();
}

