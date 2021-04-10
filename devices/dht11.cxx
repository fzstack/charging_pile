/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-11     imgcr       the first version
 */

#include "dht11.hxx"
using namespace std;

Dht11::Dht11(rt_base_t pin, std::shared_ptr<SharedThread> thread): thread(thread) {
    dev = shared_ptr<dht_device>(dht_create(pin), [](auto p){
        dht_delete(p);
    });

    timer.onRun += [this]{
        update();
    };

    timer.start();
}

void Dht11::update() {
    thread->exec([this]{
        auto ret = dht_read(dev.get());
        if(ret) {
            humidity = dht_get_humidity(dev.get()) / 10.f;
            temperature = dht_get_temperature(dev.get()) / 10.f;
        }
    });
}

float Dht11::getHumidity() {
    return humidity;
}

float Dht11::getTemperature() {
    return temperature;
}
