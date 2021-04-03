#pragma once

#include <rtthread.h>

enum class Voices: rt_uint8_t {
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
};

class Wtn6Base {
public:
    virtual void write(rt_uint8_t data) = 0;

    template<class T>
    void write(T data) {
        write(rt_uint8_t(data));
    }
};


