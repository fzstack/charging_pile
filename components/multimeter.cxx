#include "multimeter.hxx"
#include <rtthread.h>
#include <utilities/tiny_type_id.hxx>

#define LOG_TAG "com.multimeter"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

Multimeter::Multimeter(std::shared_ptr<Hlw8112> device): device(device) {
    inited.onChanged += [this](auto value) {
        if(!value) return;
        this->device->init();
        timer.onRun += [this]{
            try {
                auto& device = this->device;
                auto valA = *device->makeSess<rms_i_a>();
                auto valB = *device->makeSess<rms_i_b>();
                auto valU = *device->makeSess<rms_u>();

                device->selectChannelA();
                auto angleA = *device->makeSess<angle>() * 0.0805f;
                device->selectChannelB();
                auto angleB = *device->makeSess<angle>() * 0.0805f;

                curChA = int(calcCurrent(valA.data, curCChA));
                curChB = int(calcCurrent(valB.data, curCChB));
                vol = int(calcVoltage(valU.data, volC));
                this->angleA = angleA;
                this->angleB = angleB;
            } catch(const exception& e) {
                rt_kprintf("\033[39m %s\n\033[0m", e.what());
            }
        };

        curCChA = *this->device->makeSess<rms_i_a_c>();
        curCChB = *this->device->makeSess<rms_i_b_c>();
        volC = *this->device->makeSess<rms_u_c>();

        timer.start();
    };
}

void Multimeter::init() {
    inited = true;
}

std::shared_ptr<Multimeter::Channel> Multimeter::getChannel(ChPort port) {
    switch(port) {
     case ChPort::A:
         return std::make_shared<Channel>(curChA, vol, angleA);
         break;
     case ChPort::B:
         return std::make_shared<Channel>(curChB, vol, angleB);
         break;
    }
    return nullptr;
}

float Multimeter::calcCurrent(float val, float valc) {
    return 1.0 * val * valc / (1 << 23) / 1.7;
}

float Multimeter::calcVoltage(float val, float valc) {
    return 1.0 * val * valc / (1.0 * (1 << 22)) / 100;
}

Timer Multimeter::timer = {kTimerDurMs, kTimer};

