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
#include <devices/spi_hlw8112.hxx>
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
        Channel(Observable<std::optional<int>>& current, Observable<std::optional<int>>& voltage, Observable<std::optional<float>>& angle): current(current), voltage(voltage), angle(angle) { }
        Observer<std::optional<int>> current;
        Observer<std::optional<int>> voltage;
        Observer<std::optional<float>> angle;
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
    Observable<std::optional<float>> angleA, angleB;
    rt_uint16_t curCChA, curCChB, volC;

    Observable<bool> inited = {false};

    const static int kTimerDurMs = 1000;
    static const char* kTimer;

};

#include <utilities/singleton.hxx>
namespace Preset {
template <int R>
class Multimeter: public Singleton<Multimeter<R>>, public ::Multimeter {
    friend class Singleton<Multimeter<R>>;
    Multimeter(): ::Multimeter(SpiHlw8112<R>::get()) {}
};

template <int R>
class MultiMeterChannel {
private:
    static constexpr int MmR = R / 2;
public:
    using Owner = Multimeter<MmR>;
    static std::shared_ptr<::Multimeter::Channel> get() {
        auto mltmtr = Owner::get();
        mltmtr->init();
        auto chn = R % 2 == 0 ? ::Multimeter::ChPort::A : ::Multimeter::ChPort::B;
        return mltmtr->getChannel(chn);
    }
};
}


#endif /* APPLICATIONS2_MULTIMETER_HXX_ */
