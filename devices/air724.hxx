/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-26     imgcr       the first version
 */
#ifndef DEVICES_AIR724_HXX_
#define DEVICES_AIR724_HXX_

#include <rtthread.h>
#include <at.h>
#include <devices/output_pin.hxx>
#include <memory>
#include <string>
#include <utilities/observable.hxx>
#include <components/air_component_base.hxx>
#include <tuple>
#include <utilities/mp.hxx>

class AirEssential;
class AirHttpClient;
class AirMqttClient;

//class AirMqttClient;

class Air724: public std::enable_shared_from_this<Air724> {
    friend class AirComponentBase;
public:
    Air724(const char* atUartDevice, rt_base_t resetPin);
    void reset(rt_base_t resetMs = 1000);
    void init();

    using known_components = std::tuple<AirEssential, AirHttpClient, AirMqttClient>;
private:
    template<class T>
    struct help_make {
        template<class U>
        using cond = std::is_base_of<T, U>;
        using type = find_type_t<cond, known_components>;
    };

    template<class T>
    using help_make_t = typename help_make<T>::type;

public:
    template<class T>
    auto make() -> std::enable_if_t<!std::is_void_v<help_make_t<T>>, std::shared_ptr<T>> {
        auto inst = std::shared_ptr<help_make_t<T>>(new help_make_t<T>(shared_from_this()));
        inst->init();
        return inst;
    }

private:
    at_client_t atClient;
    OutputPin resetPin;
    Observable<bool> inited = {false};

public:
    static constexpr int
        kAtBuffSize = 512,
        kDefaultAtRespBuffSize = 128,
        kTimeoutMs = 1000;

};

#endif /* DEVICES_AIR724_HXX_ */
