#ifndef DEVICES_TTP229_HXX_
#define DEVICES_TTP229_HXX_

#include <memory>
#include <rtthread.h>
#include <utilities/thread.hxx>
#include <utilities/event.hxx>

class Ttp229 {
public:
    //90us高电平 -> 数据到达
    //16位的数据, LSB先发送
    //时钟频率范围 1K ~ 512K

    //下降沿的时候接收数据
    Ttp229(std::shared_ptr<Thread> thread, rt_base_t sclPin, rt_base_t sdoPin);

    Signals<void(rt_uint16_t)> onData;

private:
    std::shared_ptr<Thread> thread;
    rt_base_t sclPin, sdoPin;
    Event event;

    static constexpr int kTwUis = 10;
    static constexpr int kTHalfSclUis = 2;
};


#include <utilities/singleton.hxx>
namespace Preset {
class Ttp229: public Singleton<Ttp229>, public ::Ttp229 {
    friend singleton_t;
    Ttp229(): ::Ttp229(std::make_shared<Thread>(kThreadStack, kThreadPrio, kThreadTick, kThread), kSclPin, kSdoPin) {}
    static const rt_base_t kSclPin, kSdoPin;

    static const char *kThread;
    static const int kThreadStack, kThreadPrio, kThreadTick;
};
}


#endif /* DEVICES_TTP229_HXX_ */
