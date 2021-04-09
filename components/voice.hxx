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

//noti.play(Voice)

enum class VoiceFragment: rt_uint8_t {
    Slience, //20ms静音
    Port1Pluged, //一号插座已插入
    Port2Pluged, //二号插座已插入
    Port3Pluged, //三号插座已插入
    Port4Pluged, //四号插座已插入
    Port5Pluged, //五号插座已插入
    Port6Pluged, //六号插座已插入
    Port7Pluged, //七号插座已插入
    Port8Pluged, //八号插座已插入
    Port9Pluged, //九号插座已插入
    Port10Pluged, //十号插座已插入
    NoPay, //请扫码或刷卡充电
    CardDetected, //刷卡成功
    StartCharing, //开始充电
    NotAvailable, //当前设备不可用，请更换设备尝试
    Port1Unpluged, //一号插座已拔出
    Port2Unpluged,
    Port3Unpluged,
    Port4Unpluged,
    Port5Unpluged,
    Port6Unpluged,
    Port7Unpluged,
    Port8Unpluged,
    Port9Unpluged,
    Port10Unpluged,
    PlugNotReady, //请先插入充电器
    BalanceNotEnough, //当前卡余额不足，请充值
    ChargeCompleted, //充电已完成
    Plugout, //请拔出插头
    Reserved1,
    Reserved2,
    Reserved3,
    Welcome,
    Dot,
    Zero,
    One,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Hundred,
    Thousand,
    TenThousand,
    Yuan,
    No,
    Port,
    Pluged,
    Unpluged,
    PowerOverrun,
    Completed,
    Starting,
    PortSelectRequired,
    Balance,
    CardNotActivated,
    BalanceInsufficient,
    PortInvalid,
    DeviceAbnormal,
    KeyEmpty,
    KeyConfirm,
    Beep,
    //预留
};


//声音的最小单位为片段
//当一个声音加入到另一个声音的时候，他的优先级属性将无效



class Voice {
public:
    Voice& amount(int cents); //余额单位为分

private:
    static int msb10(int n);

public:
    std::list<VoiceFragment> seq = {};
private:
    static const std::array<VoiceFragment, 10> numbers;
    static const std::array<VoiceFragment, 4> bits;
};


