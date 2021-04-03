/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-01     imgcr       the first version
 */

#include "voice_notifier_lite.hxx"
#include <utilities/inner_port.hxx>

using namespace std;

VoiceNotifierLite::VoiceNotifierLite(std::shared_ptr<Wtn6Base> wtn6, std::shared_ptr<AppState> state, std::shared_ptr<UserInput> userInput) {
    state->portStateChanged += [this, wtn6](auto port, auto state) {
        auto voice = Voices::Slience;
        auto i = InnerPort{port}.get();
        switch(state) {
        case State::LoadNotInsert:
            voice = kUnpluged[i];
            break;
        case State::LoadInserted:
            voice = kPluged[i];
            break;
        case State::Charging:
            voice = Voices::StartCharing;
            break;
        case State::LoadWaitRemove:
            voice = Voices::ChargeCompleted;
            break;
        default:
            break;
        }
        if(voice != Voices::Slience) {
            wtn6->write(voice);
        }
    };

    userInput->onConfirm += [wtn6](auto port, auto cardId){
        wtn6->write(Voices::CardDetected);
    };
}

const std::array<Voices, Config::Bsp::kPortNum> VoiceNotifierLite::kUnpluged = {
    Voices::Port1Unpluged, Voices::Port2Unpluged, Voices::Port3Unpluged,
    Voices::Port4Unpluged, Voices::Port5Unpluged, Voices::Port6Unpluged,
    Voices::Port7Unpluged, Voices::Port8Unpluged, Voices::Port9Unpluged,
    Voices::Port10Unpluged,
};

const std::array<Voices, Config::Bsp::kPortNum> VoiceNotifierLite::kPluged = {
    Voices::Port1Pluged, Voices::Port2Pluged, Voices::Port3Pluged,
    Voices::Port4Pluged, Voices::Port5Pluged, Voices::Port6Pluged,
    Voices::Port7Pluged, Voices::Port8Pluged, Voices::Port9Pluged,
    Voices::Port10Pluged,
};

