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

VoiceNotifierLite::VoiceNotifierLite(std::shared_ptr<Player> player, std::shared_ptr<AppState> state, std::shared_ptr<UserInput> userInput, std::shared_ptr<Keyboard> keybaord): player(player) {
    state->cloudConnected.onChanged += [this, player](auto value) {
        if(!value) return;
        player->play(std::move(Voice{}.fragm(VoiceFragment::Welcome)));
    };

    state->portStateChanged += [this, player](auto port, auto state) {
        switch(state) {
        case State::Charging:
            player->play(std::move(Voice{}.port(port).fragm(VoiceFragment::Starting).fragm(VoiceFragment::Beep)));
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

    userInput->onCardSwipe += [player](auto cardId) {
        player->play(std::move(Voice{}.fragm(VoiceFragment::PortSelectRequired)), Player::Level::Important);
    };

    userInput->onAdminOp += [player](auto op) {
        auto voice = Voice{};
        switch(op) {
        case UserInput::AdminOp::ClearConf:
            voice.fragm(VoiceFragment::Reserved2);
            break;
        default:
            break;
        }
        player->play(std::move(voice), Player::Level::Important);
    };

    userInput->onError += [player](auto error) {
        auto voice = Voice{};

        try {
            rethrow_exception(error);
        } catch(UserInput::PortInUseError& e) {
            voice.port(e.port).fragm(VoiceFragment::Using).fragm(VoiceFragment::PleaseSelectOtherAvaliblePort);
        } catch(UserInput::PortInvalidError& e) {
            voice.fragm(VoiceFragment::PortInvalid).port(e.port);
        } catch(UserInput::Error& e) {
            switch(e.code) {
            case UserInput::ErrorCode::CardRequired:
                voice.fragm(VoiceFragment::CardSwipeOrQRRequired);
                break;
            case UserInput::ErrorCode::PortSelectRequired:
                voice.fragm(VoiceFragment::PortSelectRequired);
                break;
            default:
                break;
            }
        }


        player->play(std::move(voice), Player::Level::Important);
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

void VoiceNotifierLite::boradcast(int balance, BroadcastType type) {
    auto voice = Voice{};
    switch(type) {
    case BroadcastType::Succeed:
        voice.fragm(VoiceFragment::Balance).amount(balance);
        break;
    case BroadcastType::BalanceInsufficient:
        voice.fragm(VoiceFragment::BalanceInsufficient);
        break;
    case BroadcastType::CardNotActivated:
        voice.fragm(VoiceFragment::CardNotActivated);
        break;
    case BroadcastType::DeviceAbnormal:
        voice.fragm(VoiceFragment::DeviceAbnormal);
        break;
    default:
        break;
    }
    player->play(std::move(voice), Player::Level::Important);
}


