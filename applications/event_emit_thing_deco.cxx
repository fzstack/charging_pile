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
#include <config/app.hxx>
#include <utilities/mp.hxx>

using namespace std;

EventEmitThingDeco::EventEmitThingDeco(outer_t* outer): ThingDeco(outer) {
    inited.onChanged += [this](auto value) {
        if(!value) return;
        for(auto i = 0u; i < Config::Bsp::kPortNum; i++) {
            auto guard = getLock();
            getInfo(i).charger->stateStore->oState += [this, i](auto value) {
              if(!value) return;
              switch(*value) {
              case State::LoadInserted:
                  this->outer->onPortAccess(i);
                  break;
              case State::LoadNotInsert:
                  this->outer->onPortUnplug(i);
                  break;
              default: break;
              }
            };
        }

        getUser()->onCardSwipe += [this](auto cardId) {
            auto guard = getLock();
            rt_kprintf("processed card id is %s\n", cardId.c_str());
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

