#ifndef APPLICATIONS2_VOICE_NOTIFIER_HXX_

#define APPLICATIONS2_VOICE_NOTIFIER_HXX_

#include <devices/wtn6_base.hxx>
#include <memory>
#include "state_store_base.hxx"
#include <utilities/nested.hxx>
#include <components/user_input.hxx>
#include "last_charger.hxx"


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

struct PortSpecifiedVoice {
    Voices plugged, unplugged;
};

class VoiceNotifier: public std::enable_shared_from_this<VoiceNotifier> {
    friend void test_voice_notifier(int argc, char** argv);
public:
    VoiceNotifier(std::shared_ptr<Wtn6Base> wtn6);

    void watch(std::shared_ptr<StateStoreBase> store, PortSpecifiedVoice psv);
    void watch(std::shared_ptr<UserInput> user);

private:
    std::shared_ptr<Wtn6Base> wtn6;
    std::shared_ptr<LastCharger> last;
    //std::shared_ptr<Charger> lastInsertedcharger;
};

#include <utilities/singleton.hxx>
#include <devices/wtn6.hxx>
#include <components/state_store_preset.hxx>
#include <utilities/inner_port.hxx>
#include <utilities/nat_port.hxx>
#include <utilities/mp.hxx>
#include <config/bsp.hxx>
namespace Preset {
class VoiceNotifier: public Singleton<VoiceNotifier>, public ::VoiceNotifier {
    friend class Singleton<VoiceNotifier>;
    VoiceNotifier(): ::VoiceNotifier(Wtn6::get()) {
        watch(UserInput::get());
        for(rt_uint8_t i = 0; i < Config::Bsp::kPortNum; i++) {
            magic_switch<Config::Bsp::kPortNum>{}([&](auto v){
                watch(StateStore<decltype(v)::value>::get(), getPSV(InnerPort{i}));
            }, i);
        }
    }

    static PortSpecifiedVoice getPSV(NatPort port) {
        switch(port.get()) {
        case 1:
            return {Voices::Port1Pluged, Voices::Port1Unpluged};
        case 2:
            return {Voices::Port2Pluged, Voices::Port2Unpluged};
        case 3:
            return {Voices::Port3Pluged, Voices::Port3Unpluged};
        case 4:
            return {Voices::Port4Pluged, Voices::Port4Unpluged};
        case 5:
            return {Voices::Port5Pluged, Voices::Port5Unpluged};
        case 6:
            return {Voices::Port6Pluged, Voices::Port6Unpluged};
        case 7:
            return {Voices::Port7Pluged, Voices::Port7Unpluged};
        case 8:
            return {Voices::Port8Pluged, Voices::Port8Unpluged};
        case 9:
            return {Voices::Port9Pluged, Voices::Port9Unpluged};
        case 10:
            return {Voices::Port10Pluged, Voices::Port10Unpluged};
        default:
            return {Voices::Slience, Voices::Slience};
        }
    }
};
}

#endif /* APPLICATIONS2_VOICE_NOTIFIER_HXX_ */
