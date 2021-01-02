/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-01     imgcr       the first version
 */

#include "thing.hxx"

using namespace std;
using namespace string_literals;

Thing::Thing(array<shared_ptr<Charger>, Config::Bsp::kPortNum> chargers, shared_ptr<User> user, shared_ptr<LastCharger> last):
    infos(), user(user), last(last) {

    transform(chargers.begin(), chargers.end(), infos.begin(), [](auto charger) {
        return ChargerInfo {
            charger: charger,
        };
    });

    inited.onChanged += [this](auto value) {
        if(!value) return;
        for(auto i = 0u; i < this->infos.size(); i++) {
            auto& info =  this->infos[i];
            auto charger = info.charger;
            charger->stateStore->oState += [this, i, &info](auto value) {
                if(!value) return;
                switch(*value) {
                case State::LoadInserted:
                    onPortAccess(i); //上报端口插入事件
                    break;
                case State::LoadWaitRemove:
                    info.leftSeconds = 0; //充电完成, 将剩余计数清零
                    break;
                default:
                    break;
                }
            };
            charger->init();
        }

        this->user->onCardSwipe += [this](auto cardId) {
            if(!*this->last->oWaitingToStart) return;
            for(auto i = 0u; i < this->infos.size(); i++) {
                if(this->infos[i].charger->stateStore == **this->last->oWaitingToStart) {
                    onIcNumber(i, cardId); //上报刷卡事件
                }
            }
        };
        this->timer.start();
    };

    timer.onRun += [this](){ //定时断开充电器
        for(auto i = 0u; i < this->infos.size(); i++) {
            auto& charger = infos[i].charger;
            auto& leftSeconds = infos[i].leftSeconds;
            if(*charger->stateStore->oState == State::Charging) { //如果充电器在倒计时前被提前关闭，则以下代码不会被继续执行
                if(--leftSeconds <= 0) {
                    charger->stop(); //自动断开继电器、改变当前状态、改变灯闪烁模式、语音播报充电完成
                }
                rt_kprintf("[%d] remains %ds\n", i, leftSeconds);

            }
        }
    };
}

void Thing::init() {
    inited = true;
}

void Thing::control(int port, int timerId, int minutes) {
    auto& info = infos[port]; //NOTE: std::array会自动进行边界检查
    info.charger->start();
    info.timerId = timerId;
    info.leftSeconds = minutes * 60;
}

void Thing::stop(int port, int timerId) {
    auto& info = infos[port];
    info.charger->stop();
    info.timerId = timerId;
}


