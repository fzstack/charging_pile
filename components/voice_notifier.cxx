#include "voice_notifier.hxx"
#include <rtthread.h>

//#include "app_config.hxx"
//#include "charger.hxx"
//#include "user.hxx"

using namespace std;

VoiceNotifier::VoiceNotifier(shared_ptr<Wtn6Base> wtn6): wtn6(wtn6) {

}

void VoiceNotifier::watch(shared_ptr<StateStoreBase> store, PortSpecifiedVoice psv) {
    store->oState += [=](auto value) {
        if(!value) return;
        switch(*value) {
        case State::LoadInserted:
            wtn6->write(psv.plugged);
            break;
        case State::Charging:
            wtn6->write(Voices::StartCharing);
            break;
        case State::LoadNotInsert:
            //如果是刚开机则不播报
            if(rt_tick_get() < 500) break;
            wtn6->write(psv.unplugged);
            break;
        case State::LoadWaitRemove:
            wtn6->write(Voices::ChargeCompleted);
            break;
        default:
            break;
        }
    };
}

void VoiceNotifier::watch(shared_ptr<UserInput> user) {
    user->onConfirm += [this](auto port, auto cardId) {
        //TODO: 判断是否刷卡
        //wtn6->write(Voices::PlugNotReady);
        wtn6->write(Voices::CardDetected);
    };
}


