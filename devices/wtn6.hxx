#ifndef DEVICES_WTN6_HXX_
#define DEVICES_WTN6_HXX_

#include <rtthread.h>
#include "wtn6_base.hxx"

class Wtn6: public Wtn6Base {
public:
    Wtn6(rt_base_t dataPin, rt_base_t busyPin);

    bool isBusy();
    virtual void write(rt_uint8_t data) override;


private:
    void writeBit(bool bit);

private:
    const rt_base_t dataPin, busyPin;
};

#include <utilities/singleton.hxx>
namespace Preset {
class Wtn6: public Singleton<Wtn6>, public ::Wtn6 {
    friend singleton_t;
    Wtn6(): ::Wtn6(kDataPin, kBusyPin) {}
    static const rt_base_t kDataPin, kBusyPin;
};
}


#endif /* DEVICES_WTN6_HXX_ */
