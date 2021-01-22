/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-30     imgcr       the first version
 */

#include "ali_cloud.hxx"
#include <components/air_components.hxx>
#include <config/app.hxx>

using namespace std;

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
                           {"state", int(state)},
                        });
                    } else if(auto err = get_if<exception_ptr>(&result)) {
                        r(*err);
                    }
                }, params["port"], params["timer_id"], params["minutes"]);
            } catch(const exception& e) {
                r(std::current_exception());
            }
        };

        this->device->services["stop"] += [this](auto r, const auto params)  {
            try {
                onStop([r](auto result) mutable {
                    if(auto state = get_if<ServiceResult::Value>(&result)) {
                        r(Json {
                           {"state", int(state)},
                        });
                    } else if(auto err = get_if<exception_ptr>(&result)) {
                        r(*err);
                    }
                }, params["port"], params["timer_id"]);
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
                }, params["current_limit"], params["upload_thr"], params["fused_thr"]);
            } catch(const exception& e) {
                r(std::current_exception());
            }
        };

        this->device->login(Config::App::cloudDeviceName, Config::App::cloudProductKey, Config::App::cloudDeviceSecret);

        auto ess = this->air->make<AirEssential>();
        setIccid(ess->getIccid());

        signal += this->device->thread->post([=](){
            auto ess = this->air->make<AirEssential>();
            setSignal(ess->getCsq());
        });

        Cloud::init();
        this->isInited = true;
    };
}

void AliCloud::init() {
    inited = true;
}

void AliCloud::setCurrentData(std::array<CurrentData, Config::Bsp::kPortNum>& data) {
    if(!isInited) return;
    auto value = Json::array({});
    for(const auto& item: data) {
        value.push_back({
            {"port", item.port},
            {"timer_id", item.timerId},
            {"left_minutes", item.leftMinutes},
            {"state", item.state},
            {"current", item.current},
            {"voltage", item.voltage},
            {"consumption", item.consumption},
            {"fuse", item.fuse},
        });
    }
    device->set("current_data", value);
}

void AliCloud::setIccid(std::string_view iccid) {
    if(!isInited) return;
    device->set("iccid", iccid);
}

void AliCloud::setSignal(int signal) {
    if(!isInited) return;
    device->set("signal", signal);
}

void AliCloud::emitPortAccess(int port) {
    if(!isInited) return;
    device->emit("port_access", {
        {"port", port},
    });
}

void AliCloud::emitIcNumber(int port, std::string_view icNumber) {
    if(!isInited) return;
    device->emit("ic_number", {
       {"port", port},
       {"ic_number", icNumber},
    });
}

void AliCloud::emitCurrentLimit(int port) {
    if(!isInited) return;
    device->emit("current_limit", {
       {"port", port},
    });
}

void AliCloud::setSignalInterval() {
    if(!isInited) return;
    signal();
}

