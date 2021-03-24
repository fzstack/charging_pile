/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-01     imgcr       the first version
 */
#ifndef COMPONENTS_USER_HXX_
#define COMPONENTS_USER_HXX_

#include <devices/rc522_preset.hxx>
#include <components/keyboard.hxx>
#include <utilities/signals.hxx>
#include <string>

class User {
public:
    User() = default;
    void watch(std::shared_ptr<Rc522Base> rc522);
    void watch(std::shared_ptr<Keyboard> keyboard);

    Signals<void(std::string)> onCardSwipe = {};
    Signals<void(int port, std::string cardId)> onConfirm = {};

private:
    std::string lastCard;
    int currPort = 0;
};

#include <utilities/singleton.hxx>
namespace Preset {
class User: public Singleton<User>, public ::User {
    friend singleton_t;
    User(): ::User() {
        watch(Rc522::get());
        watch(Keyboard::get());
    }
};
}

#endif /* COMPONENTS_USER_HXX_ */
