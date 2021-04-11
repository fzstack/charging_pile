/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-11     imgcr       the first version
 */
#pragma once
#include <rtthread.h>
#include <memory>
extern "C" {
#include <dhtxx.h>
}
#include <utilities/shared_thread.hxx>
#include <components/timer.hxx>
#include <utilities/observer.hxx>

class Dht11 {
public:
    Dht11(rt_base_t pin, std::shared_ptr<SharedThread> thread);
private:
    void update();
    std::shared_ptr<SharedThread> thread;
    std::shared_ptr<dht_device> dev;
    Timer timer = {1000, "dht11"};
    Observable<float> humidity = 0, temperature = 0;
public:
    Observer<float> oHumidity = {humidity}, oTemperature = {temperature};
};

#include <utilities/singleton.hxx>
#include <config/bsp.hxx>
namespace Preset {
class Dht11: public Singleton<Dht11>, public ::Dht11 {
    friend singleton_t;
    Dht11(): ::Dht11(kPin, SharedThread<Priority::High>::get()) {}
    static const rt_base_t kPin;
};
}
