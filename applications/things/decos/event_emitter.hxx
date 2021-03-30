#pragma once

#include "base.hxx"
#include <utilities/observable.hxx>

namespace Things::Decos {
/**
 * @description: 事件上报功能类
 */
class EventEmitter: public Base {
    friend outer_t;
    using Base::Base;
    virtual void onStateChanged(InnerPort port, State::Value state) override;
};
}
