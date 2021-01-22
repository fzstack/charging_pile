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

#include <array>
#include <components/charger.hxx>
#include <components/last_charger.hxx>
#include <components/user.hxx>
#include <config/bsp.hxx>
#include <config/app.hxx>
#include <memory>
#include <utilities/signals.hxx>
#include <stdexcept>
#include <components/timer.hxx>
#include "thing_deco.hxx"
#include <Mutex.h>

class ThingDeco;

struct ChargerInfo {
    std::shared_ptr<Charger> charger = {};
    int timerId = 0;
    int leftSeconds = 0;
    float consumption = 0; //AxV
};

class Thing {
public:
    friend class ThingDeco;
    Thing(std::array<std::shared_ptr<Charger>, Config::Bsp::kPortNum> chargers, std::shared_ptr<User> user, std::shared_ptr<LastCharger> last);
    void init();

    template<class T>
    auto addDeco() -> std::enable_if_t<std::is_base_of_v<ThingDeco, T>> {
        auto deco = std::shared_ptr<T>(new T(this));
        decos.push_back(deco);
    }
    void control(int port, int timerId, int minutes);
    void stop(int port, int timerId);
    void config(int currentLimit, int uploadThr, int fuzedThr);

    Signals<void(int port)> onPortAccess;
    Signals<void(int port)> onPortUnplug;
    Signals<void(int port, std::string icNumber)> onIcNumber;
    Signals<void(int port)> onCurrentLimit;
    Signals<void()> onCurrentData;

    Signals<std::array<CurrentData, Config::Bsp::kPortNum>()> getCurrentData;

private:
    std::array<ChargerInfo, Config::Bsp::kPortNum> infos;
    std::shared_ptr<User> user;
    std::shared_ptr<LastCharger> last;
    std::list<std::shared_ptr<ThingDeco>> decos = {};
    rtthread::Mutex mutex = {kMutex};

    Observable<bool> inited = {false};

    static const char* kMutex;
};

struct thing_error: public std::runtime_error {
    using std::runtime_error::runtime_error;
};

#include <utilities/singleton.hxx>
namespace Preset {
class Thing: public Singleton<Thing>, public ::Thing {
    friend class Singleton<Thing>;
    Thing();

    static std::array<std::shared_ptr<::Charger>, Config::Bsp::kPortNum> getChargers();
};
}

#endif /* APPLICATIONS_THING_HXX_ */
