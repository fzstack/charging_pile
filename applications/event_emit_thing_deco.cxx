/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-03     imgcr       the first version
 */

#include "event_emit_thing_deco.hxx"

EventEmitThingDeco::EventEmitThingDeco(outer_t* outer): ThingDeco(outer) {
    inited.onChanged += [this](auto value) {
        if(!value) return;
        for(auto i = 0u; i < Config::Bsp::kPortNum; i++) {
            getInfo(i).charger->stateStore->oState += [this, i](auto value) {
              if(value != State::LoadInserted) return;
              this->outer->onPortAccess(i);
            };
        }

        getUser()->onCardSwipe += [this](auto cardId) {
            auto waitingToStart = *getLast()->oWaitingToStart;
            if(!waitingToStart) return;
            for(auto i = 0u; i < Config::Bsp::kPortNum; i++) {
                if(getInfo(i).charger->stateStore == waitingToStart) {
                    this->outer->onIcNumber(i, cardId);
                }
            }
        };
    };
}

void EventEmitThingDeco::init() {
    inited = true;
}

