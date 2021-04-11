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
#include <rtthread.h>
#include <memory>
#include <list>
#include <variant>
#include <array>
#include <utilities/nat_port.hxx>

//noti.play(Voice)

enum class VoiceFragment: rt_uint8_t {
    Slience,
    Welcome,
    Dot, Zero, One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
    Ten, Hundred, Thousand, TenThousand,
    Yuan, No,
    Port,
    Pluged, Unpluged, PowerOverrun, Completed, StopCharged, Starting, PortRemoved,
    PortSelectRequired, CardSwipeOrQRRequired, CardSwipeOk,
    Balance, CardNotActivated, BalanceInsufficient, PortInvalid, DeviceAbnormal,
    KeyEmpty, KeyConfirm, Beep,
    Welcome2,
};


//声音的最小单位为片段
//当一个声音加入到另一个声音的时候，他的优先级属性将无效



class Voice {
public:
    Voice& amount(int cents); //余额单位为分
    Voice& integer(int number);
    Voice& port(NatPort port);
    Voice& fragm(VoiceFragment fragm);
private:
    static int msb10(int n);

public:
    std::list<VoiceFragment> seq = {};
private:
    static const std::array<VoiceFragment, 10> numbers;
    static const std::array<VoiceFragment, 4> bits;
};


