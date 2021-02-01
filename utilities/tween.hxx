/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-01     imgcr       the first version
 */
#pragma once
#include "observable.hxx"
#include <optional>
#include "err.hxx"
#include <components/timer.hxx>
#include <Mutex.h>
#include <Lock.h>

template<class T>
class Tween {
public:
    Tween(T initVal, int duration, int freq): durationMs(duration), freqHz(freq), initVal(initVal), timer(std::make_shared<Timer>(1000 / freq, "tween")) {
        timer->onRun += [this]{
            auto guard = rtthread::Lock(mutex);
            curPos += posStep;
            if(curPos >= 1) {
                curPos = 1;
                currVal = *targetVal;
                timer->stop();
            } else {
                currVal = update(this->initVal, *targetVal, ease(curPos));
            }
        };
    }

    void setTargetVal(T t) {
        auto guard = rtthread::Lock(mutex);
        if(targetVal && *targetVal == t)
            return;

        initVal = *currVal;
        targetVal = t;
        curPos = 0;
        if(!timer->isRunning()) {
            timer->start();
        }
    }

protected:
    virtual T update(T& initVal, T& targetVal, float pos) = 0;

    //从一个值到另一个值的渐变,可以设定渐变时长 缓动函数 渐变频率

    //值域0..1 定义域0..1
    virtual float ease(float x) {
        return x;
    }

private:
    int durationMs;
    const int freqHz;
    T initVal;
    std::shared_ptr<Timer> timer;
    float curPos = 0;
    float posStep =  1000.f / durationMs / freqHz;
    Observable<T> currVal = {initVal};
    std::optional<T> targetVal = {};
public:
    Observer<T> oCurrVal = {currVal};
private:
    rtthread::Mutex mutex = {"tween"};
};


