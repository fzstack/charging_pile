/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-09     imgcr       the first version
 */

#include "player.hxx"
#include <Lock.h>

using namespace rtthread;
Player::Player() {
    timer.onRun += [this]{
        auto guard = Lock(mutex);
        if(done) return;
        //如果有打断标志或者当前未在播放，则播放下一条
        if(fIrrupt || !isFragmentPlaying()) {
            fIrrupt = false;
            //如果当前已经是end了，设done为ture
            if(currIt == currVoice.seq.end()) {
                done = true;
            } else {
                playFragment(*currIt);
                ++currIt;
            }
        }
    };
    timer.start();
}

//done的条件是语段播放完成并且it == end()
void Player::play(Voice&& voice, Level level) {
    auto guard = Lock(mutex);
    if(done || int(level) <= int(currLevel)) {
        fIrrupt = true; //NOTE: 需要这个吗？
        currVoice = voice;
        currLevel = level;
        currIt = currVoice.seq.begin();
        done = false;
    }
}

//restart
