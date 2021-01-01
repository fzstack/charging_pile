/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-01     imgcr       the first version
 */

#include "last_charger.hxx"

using namespace std;

LastCharger::LastCharger(): oWaitingToStart(waitingToStart) {

}

void LastCharger::watch(shared_ptr<StateStoreBase> store) {
    store->oState += [=](auto value) {
        if(!value) return;
        if(*value == State::LoadInserted) {
            waitingToStart = store;
        } else if(*waitingToStart && **waitingToStart == store && *value != State::LoadInserted) {
            waitingToStart = nullopt;
        }
    };
}

