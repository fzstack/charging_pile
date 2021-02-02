/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-26     imgcr       the first version
 */
#ifndef APPLICATIONS2_VOICE_NOTIFIER_HXX_
#define APPLICATIONS2_VOICE_NOTIFIER_HXX_

#include <devices/wtn6.hxx>
#include <memory>
#include "state_store_base.hxx"
#include <utilities/nested.hxx>
#include <components/user.hxx>
#include "last_charger.hxx"


enum class Voices: rt_uint8_t {
    Slience, //20ms静音
    Port0Pluged, //一号插座已插入
    Port1Pluged, //二号插座已插入
    Port2Pluged, //三号插座已插入
    Port3Pluged, //四号插座已插入
    Port4Pluged, //五号插座已插入
    Port5Pluged, //六号插座已插入
    Port6Pluged, //七号插座已插入
    Port7Pluged, //八号插座已插入
    Port8Pluged, //九号插座已插入
    Port9Pluged, //十号插座已插入
    NoPay, //请扫码或刷卡充电
    CardDetected, //刷卡成功
    StartCharing, //开始充电
    NotAvailable, //当前设备不可用，请更换设备尝试
    Port0Unpluged, //一号插座已拔出
    Port1Unpluged,
    Port2Unpluged,
    Port3Unpluged,
    Port4Unpluged,
    Port5Unpluged,
    Port6Unpluged,
    Port7Unpluged,
    Port8Unpluged,
    Port9Unpluged,
    PlugNotReady, //请先插入充电器
    BalanceNotEnough, //当前卡余额不足，请充值
    ChargeCompleted, //充电已完成
    Plugout, //请拔出插头
    Reserved1,
    Reserved2,
    Reserved3,
};

struct PortSpecifiedVoice {
    Voices plugged, unplugged;
};

class VoiceNotifier: public std::enable_shared_from_this<VoiceNotifier> {
    friend void test_voice_notifier(int argc, char** argv);
public:
    VoiceNotifier(std::shared_ptr<Wtn6> wtn6);

    void watch(std::shared_ptr<StateStoreBase> store, PortSpecifiedVoice psv);
    void watch(std::shared_ptr<User> user);
    void watch(std::shared_ptr<LastCharger> last);

private:
    std::shared_ptr<Wtn6> wtn6;
    std::shared_ptr<LastCharger> last;
    //std::shared_ptr<Charger> lastInsertedcharger;
};

#include <utilities/singleton.hxx>
namespace Preset {
class VoiceNotifier: public Singleton<VoiceNotifier>, public ::VoiceNotifier {
    friend class Singleton<VoiceNotifier>;
    VoiceNotifier(): ::VoiceNotifier(Wtn6::get()) {
        watch(User::get());
        watch(LastCharger::get());
    }
};
}

#endif /* APPLICATIONS2_VOICE_NOTIFIER_HXX_ */
