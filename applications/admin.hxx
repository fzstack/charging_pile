/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-06     imgcr       the first version
 */
#pragma once
#include <components/user_input.hxx>
#include <utilities/shared_thread.hxx>
#include <components/fal_persistent_storage.hxx>

class Admin {
public:
    Admin(std::shared_ptr<UserInput> input, std::shared_ptr<SharedThread> thread, std::shared_ptr<FalPersistentStorage> storage);


private:
    std::shared_ptr<UserInput> input;
    std::shared_ptr<SharedThread> thread;
    std::shared_ptr<FalPersistentStorage> storage;
};

#include <utilities/singleton.hxx>
namespace Preset {
class Admin: public Singleton<Admin>, public ::Admin {
    friend singleton_t;
    Admin(): ::Admin(UserInput::get(), SharedThread<Priority::Low>::get(), FalPersistentStorage::get()) {

    }
};
}
