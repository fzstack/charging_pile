/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-30     imgcr       the first version
 */
#include "virtual_load_detector.hxx"
#include <Lock.h>

using namespace rtthread;

VirtualLoadDetector::VirtualLoadDetector(
  std::shared_ptr<LoadDetector> physical,
  std::shared_ptr<Relay> relay,
  std::shared_ptr<Multimeter::Channel> multimeterChannel
):physical(physical),
  relay(relay),
  multimeterChannel(multimeterChannel),
  state(),
  oState(state),
  timer(kRelayOffStateUpdateDelayMs, kTimer, RT_TIMER_FLAG_ONE_SHOT) {
    inited.onChanged += [this](auto value) {
        if(!value) return;
        this->physical->oState += [this](auto value){ update(); };
        this->relay->value.onChanged += [this](auto value) {
            if(value == Relay::Off) {
                timer.start();
                return;
            }
            update();
        };
        this->multimeterChannel->current += [this](auto value) { update(); };
        this->multimeterChannel->voltage += [this](auto value) { update(); };
        this->timer.onRun += [this]() {
            rt_kprintf("vlod timer runned\n");
            update();
        };
        update();
    };
}

void VirtualLoadDetector::init() {
    inited = true;
}

void VirtualLoadDetector::update() {
    auto lock = Lock{mutex};

    if(*relay->value == Relay::On) {
        state = true;
        return;
    }

    if(timer.isRunning()) return;
    state = this->physical->oState.value();
}

Mutex VirtualLoadDetector::mutex = {kMutex};
