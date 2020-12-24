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



VoiceNotifier::VoiceNotifier(std::shared_ptr<Wtn6> wtn6): wtn6(wtn6) {

}

#ifdef RUN_APP
void VoiceNotifier::watch(std::shared_ptr<Charger> charger, Chargers chargers) {
    auto self(shared_from_this());
    charger->loadDetector->oState += [=](auto state) {
        if(!state.has_value())
            return;

        Voices loadInserted, loadRemoved;
        switch(chargers) {
            case Chargers::A:
                loadInserted = Voices::PortAPluged;
                loadRemoved = Voices::PortAUnpluged;
                break;
            case Chargers::B:
                loadInserted = Voices::PortBPluged;
                loadRemoved = Voices::PortBUnpluged;
                break;
        }

        if(*state) {
            lastInsertedcharger = charger;
        } else {
            if(lastInsertedcharger == charger) {
                lastInsertedcharger = nullptr;
            }
        }

        wtn6->write(*state ? loadInserted : loadRemoved);
    };

    charger->relay->oIsOn += [=](auto isOn) {
        if(!isOn.has_value())
            return;

        wtn6->write(*isOn ? Voices::StartCharing : Voices::ChargeCompleted);
    };
}

void VoiceNotifier::watch(std::shared_ptr<User> user) {
    auto self(shared_from_this());
    user->onCardSwiped += [this, self]() {
        if(lastInsertedcharger == nullptr || lastInsertedcharger->relay->oIsOn.value().value_or(false)) {
            wtn6->write(Voices::PlugNotReady);
        } else {
            wtn6->write(Voices::CardDetected);
        }
    };
}
#endif

