/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-05-06     imgcr       the first version
 */

#include "admin.hxx"

Admin::Admin(std::shared_ptr<UserInput> input, std::shared_ptr<SharedThread> thread, std::shared_ptr<FalPersistentStorage> storage)
  : input(input), thread(thread), storage(storage) {
    input->onAdminOp += [this](auto op){
        switch(op) {
        case UserInput::AdminOp::ClearConf:
            this->thread->exec([this](){
                this->storage->reset();
            });
            break;
        default:
            break;
        }
    };
}
