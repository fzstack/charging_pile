/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-01     imgcr       the first version
 */
#ifndef APPLICATIONS_THING_HXX_
#define APPLICATIONS_THING_HXX_

#include <components/charger.hxx>
#include <components/last_charger.hxx>
#include <components/user.hxx>
#include <memory>
#include <stdexcept>
#include <components/timer.hxx>
#include "things/decos/base.hxx"
#include <Mutex.h>
#include "thing_base.hxx"

namespace Things::Decos {
class Base;
}

struct ChargerInfo {
    std::shared_ptr<Charger> charger = {};
    int timerId = 0;
    int leftSeconds = 0;
    float consumption = 0; //AxV
};

class Thing: public ThingBase {
public:
    friend class Things::Decos::Base;
    Thing(std::array<std::shared_ptr<Charger>, Config::Bsp::kPortNum> chargers, std::shared_ptr<User> user, std::shared_ptr<LastCharger> last);
    void init();

    template<class T>
    auto addDeco() -> std::enable_if_t<std::is_base_of_v<Things::Decos::Base, T>> {
        auto deco = std::shared_ptr<T>(new T(this));
        decos.push_back(deco);
    }
    virtual void control(int port, int timerId, int minutes) override;
    virtual void stop(int port, int timerId) override;
    virtual void config(int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) override;


private:
    std::array<ChargerInfo, Config::Bsp::kPortNum> infos;
    std::shared_ptr<User> user;
    std::shared_ptr<LastCharger> last;
    std::list<std::shared_ptr<Things::Decos::Base>> decos = {};
    rtthread::Mutex mutex = {kMutex};

    Observable<bool> inited = {false};

    static const char* kMutex;
};

struct thing_error: public std::runtime_error {
    using std::runtime_error::runtime_error;
};

#endif /* APPLICATIONS_THING_HXX_ */
