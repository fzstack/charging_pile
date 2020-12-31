/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-08-25     imgcr       the first version
 */
#ifndef APPLICATIONS2_MULTIMETER_HXX_
#define APPLICATIONS2_MULTIMETER_HXX_

#include <utilities/static_ctor.hxx>
#include <utilities/observable.hxx>
#include <devices/hlw8112.hxx>
#include <devices/hlw8112_regs.hxx>
#include <memory>
#include <tuple>
#include <optional>

using namespace Hlw8112ns::Regs;

class Multimeter {
    //class Channel
public:
    Multimeter(std::shared_ptr<Hlw8112> device);
    void init();

    class Channel {
    public:
        Channel(Observable<std::optional<int>>& current, Observable<std::optional<int>>& voltage): current(current), voltage(voltage) { }
        Observer<std::optional<int>> current;
        Observer<std::optional<int>> voltage;
    };
    friend class Channel;

    enum class ChPort {
        A, B,
    };
    std::shared_ptr<Channel> getChannel(ChPort port);

private:
    float calcCurrent(float val, float valc);
    float calcVoltage(float val, float valc);

private:
    std::shared_ptr<Hlw8112> device;
    std::shared_ptr<rt_timer> timer;
    Observable<std::optional<int>> curChA, curChB, vol;
    rt_uint16_t curCChA, curCChB, volC;

    Observable<bool> inited = {false};

    const static int kTimerDurMs = 1000;
    static const char* kTimer;

};

#include <utilities/singleton.hxx>
namespace Preset {
class Multimeter: public Singleton<Multimeter>, public ::Multimeter {
    friend class Singleton<Multimeter>;
    Multimeter(): ::Multimeter(Hlw8112::get()) {}
public:
    template<int R>
    static constexpr Multimeter::ChPort getPort() {
        switch(R) {
        case 0:
            return Multimeter::ChPort::B;
        case 1:
            return Multimeter::ChPort::A;
        default:
            throw std::runtime_error{"resource id out of range"};
        }
    }
};
}


#endif /* APPLICATIONS2_MULTIMETER_HXX_ */
