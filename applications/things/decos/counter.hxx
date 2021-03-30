#pragma once

#include "base.hxx"
#include <utilities/observable.hxx>

namespace Things::Decos {
/**
 * @description: 倒计时功能类
 */
class Counter: public Base {
    friend outer_t;
    Counter(outer_t* outer);
    virtual void init() override;
    virtual void onStateChanged(InnerPort port, State::Value state) override;

    Timer timer = {1000, kTimer};
    static const char* kTimer;
};
}
