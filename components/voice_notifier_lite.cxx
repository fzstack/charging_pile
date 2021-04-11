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
#include <components/voice.hxx>

using namespace std;

//需求:
//1. 声音分多种优先级
//2. 声音可以由片段合成

VoiceNotifierLite::VoiceNotifierLite(std::shared_ptr<Player> player, std::shared_ptr<AppState> state, std::shared_ptr<UserInput> userInput, std::shared_ptr<Keyboard> keybaord) {
    state->portStateChanged += [this, player](auto port, auto state) {
        switch(state) {
        case State::Charging:
            player->play(std::move(Voice{}.port(port).fragm(VoiceFragment::StartCharing)));
            break;
        case State::LoadWaitRemove:
            player->play(std::move(Voice{}.port(port).fragm(VoiceFragment::StopCharged)));
            break;
        default:
            break;
        }
    };

    userInput->onConfirm += [player](auto port, auto cardId){
        player->play(std::move(Voice{}.fragm(VoiceFragment::CardSwipeOk)), Player::Level::Important);
    };

    keybaord->oValue += [player](auto value) {
        if(!value) return;
        if(Keyboard::isNumberKey(*value)) {
            auto number = Keyboard::keyToNumber(*value);
            player->play(std::move(Voice{}.integer(number)));
        } else {
            auto frgma = VoiceFragment::Slience;
            switch(*value) {
            case Keyboard::Keys::Ok:
                frgma = VoiceFragment::KeyConfirm; break;
            case Keyboard::Keys::Ret:
                frgma = VoiceFragment::KeyEmpty; break;
            default: break;
            }
            if(frgma != VoiceFragment::Slience) {
                player->play(std::move(Voice{}.fragm(frgma)));
            }
        }
    };
}


