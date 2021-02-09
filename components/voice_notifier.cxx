/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-26     imgcr       the first version
 */

#include "voice_notifier.hxx"
#include <rtthread.h>

//#include "app_config.hxx"
//#include "charger.hxx"
//#include "user.hxx"

using namespace std;

VoiceNotifier::VoiceNotifier(shared_ptr<Wtn6Base> wtn6): wtn6(wtn6) {

}

void VoiceNotifier::watch(shared_ptr<StateStoreBase> store, PortSpecifiedVoice psv) {
    store->oState += [=](auto value) {
        if(!value) return;
        switch(*value) {
        case State::LoadInserted:
            wtn6->write(psv.plugged);
            break;
        case State::Charging:
            wtn6->write(Voices::StartCharing);
            break;
        case State::LoadNotInsert:
            //如果是刚开机则不播报
            if(rt_tick_get() < 500) break;
            wtn6->write(psv.unplugged);
            break;
        case State::LoadWaitRemove:
            wtn6->write(Voices::ChargeCompleted);
            break;
        default:
            break;
        }
    };
}

void VoiceNotifier::watch(shared_ptr<User> user) {
    user->onCardSwipe += [this](auto cardId) {
        //TODO: 判断是否刷卡
        if(!last) return; //没有watch之前啥都不做
        if(*last->oWaitingToStart == nullopt) {
            wtn6->write(Voices::PlugNotReady);
        } else {
            wtn6->write(Voices::CardDetected);
        }
    };
}

void VoiceNotifier::watch(shared_ptr<LastCharger> last) {
    this->last = last;
}


