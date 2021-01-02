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
#include <memory>
#include <utilities/signals.hxx>
#include <stdexcept>
#include <components/timer.hxx>

class Thing {
public:
    Thing(std::array<std::shared_ptr<Charger>, Config::Bsp::kPortNum> chargers, std::shared_ptr<User> user, std::shared_ptr<LastCharger> last);
    void init();
    void control(int port, int timerId, int minutes);
    void stop(int port, int timerId);

    Signals<void(int port)> onPortAccess;
    Signals<void(int port, std::string icNumber)> onIcNumber;
private:
    struct ChargerInfo {
        std::shared_ptr<Charger> charger = {};
        int timerId = 0;
        int leftSeconds = 0;
    };

    std::array<ChargerInfo, Config::Bsp::kPortNum> infos;
    std::shared_ptr<User> user;
    std::shared_ptr<LastCharger> last;

    Observable<bool> inited = {false};
    Timer timer = {1000, kTimer};

    static const char* kTimer;
};

struct thing_error: public std::runtime_error {
    using std::runtime_error::runtime_error;
};

#include <utilities/singleton.hxx>
namespace Preset {
class Thing: public Singleton<Thing>, public ::Thing {
    friend class Singleton<Thing>;
    Thing(): ::Thing({Charger<0>::get(), Charger<1>::get()}, User::get(), LastCharger::get()) {
        init();
    }
};
}

#endif /* APPLICATIONS_THING_HXX_ */
