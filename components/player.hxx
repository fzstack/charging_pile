/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-09     imgcr       the first version
 */
#pragma once
#include "voice.hxx"
#include <components/timer.hxx>
#include <Mutex.h>
#include <list>

class Player {
public:
    Player();

    enum class Level {
        Important, Ignorable
    };

    void play(Voice&& voice, Level level = Level::Ignorable);
protected:
    virtual void playFragment(VoiceFragment fragment) = 0;
    virtual bool isFragmentPlaying() = 0;
private:
    Timer timer = {100, "player"};
    rtthread::Mutex mutex = {"player"};
    Voice currVoice;
    Level currLevel;
    typename std::list<VoiceFragment>::iterator currIt;
    bool done = true;
    bool fIrrupt = false;
};

