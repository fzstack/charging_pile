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

//需求:
//1. 声音分多种优先级
//2. 声音可以由片段合成

VoiceNotifierLite::VoiceNotifierLite(std::shared_ptr<Wtn6Base> wtn6, std::shared_ptr<AppState> state, std::shared_ptr<UserInput> userInput, std::shared_ptr<Keyboard> keybaord) {
    state->portStateChanged += [this, wtn6](auto port, auto state) {
        auto voice = VoiceFragment::Slience;
        //auto i = InnerPort{port}.get();
        switch(state) {
        case State::LoadNotInsert:
            //voice = kUnpluged[i];
            break;
        case State::LoadInserted:
            //voice = kPluged[i];
            break;
        case State::Charging:
            voice = VoiceFragment::StartCharing;
            break;
        case State::LoadWaitRemove:
            voice = VoiceFragment::ChargeCompleted;
            break;
        default:
            break;
        }
        if(voice != VoiceFragment::Slience) {
            wtn6->write(voice);
        }
    };

    userInput->onConfirm += [wtn6](auto port, auto cardId){
        wtn6->write(VoiceFragment::CardDetected);
    };

//    keybaord->oValue += [wtn6](auto value) {
//        if(!value) return;
//        auto voice = Voices::Slience;
//        switch(*value) {
//        case Keyboard::Keys::K0:
//            voice = Voices::Port10Pluged;
//            break;
//        case Keyboard::Keys::K1:
//            voice = Voices::Port1Pluged;
//            break;
//        case Keyboard::Keys::K2:
//            voice = Voices::Port2Pluged;
//            break;
//        case Keyboard::Keys::K3:
//            voice = Voices::Port3Pluged;
//            break;
//        case Keyboard::Keys::K4:
//            voice = Voices::Port4Pluged;
//            break;
//        case Keyboard::Keys::K5:
//            voice = Voices::Port5Pluged;
//            break;
//        case Keyboard::Keys::K6:
//            voice = Voices::Port6Pluged;
//            break;
//        case Keyboard::Keys::K7:
//            voice = Voices::Port7Pluged;
//            break;
//        case Keyboard::Keys::K8:
//            voice = Voices::Port8Pluged;
//            break;
//        case Keyboard::Keys::K9:
//            voice = Voices::Port9Pluged;
//            break;
//        case Keyboard::Keys::Ret:
//            voice = Voices::Reserved1;
//            break;
//        case Keyboard::Keys::Ok:
//            voice = Voices::Reserved2;
//            break;
//        }
//        if(voice != Voices::Slience) {
//            rt_kprintf("will voice: %d\n", voice);
//            wtn6->write(voice);
//        }
//    };
}

const std::array<VoiceFragment, Config::Bsp::kPortNum> VoiceNotifierLite::kUnpluged = {
    VoiceFragment::Port1Unpluged, VoiceFragment::Port2Unpluged, VoiceFragment::Port3Unpluged,
    VoiceFragment::Port4Unpluged, VoiceFragment::Port5Unpluged, VoiceFragment::Port6Unpluged,
    VoiceFragment::Port7Unpluged, VoiceFragment::Port8Unpluged, VoiceFragment::Port9Unpluged,
    VoiceFragment::Port10Unpluged,
};

const std::array<VoiceFragment, Config::Bsp::kPortNum> VoiceNotifierLite::kPluged = {
    VoiceFragment::Port1Pluged, VoiceFragment::Port2Pluged, VoiceFragment::Port3Pluged,
    VoiceFragment::Port4Pluged, VoiceFragment::Port5Pluged, VoiceFragment::Port6Pluged,
    VoiceFragment::Port7Pluged, VoiceFragment::Port8Pluged, VoiceFragment::Port9Pluged,
    VoiceFragment::Port10Pluged,
};

