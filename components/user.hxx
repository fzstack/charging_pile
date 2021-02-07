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
#include <utilities/signals.hxx>
#include <string>

class User {
public:
    User() = default;
    void watch(std::shared_ptr<Rc522Base> rc522);
    Signals<void(std::string)> onCardSwipe = {};
};

#include <utilities/singleton.hxx>
namespace Preset {
class User: public Singleton<User>, public ::User {
    friend singleton_t;
    User(): ::User() {
        watch(Rc522::get());
    }
};
}

#endif /* COMPONENTS_USER_HXX_ */
