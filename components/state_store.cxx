/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-30     imgcr       the first version
 */

#include "state_store.hxx"

void StateStore::watch(std::shared_ptr<Relay> relay) {
    //relay->getHandler().onChanged += signal(&StateStore::update, this);
}

void StateStore::watch(std::shared_ptr<Multimeter::Channel> multimeterChannel) {
    //multimeterChannel->current += signal(&StateStore::update, this);
    //multimeterChannel->voltage += signal(&StateStore::update, this);
}

void StateStore::watch(std::shared_ptr<LoadDetector> loadDetector) {

}

void StateStore::update() {

}

