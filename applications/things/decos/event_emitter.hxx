#pragma once

#include "base.hxx"
#include <utilities/observable.hxx>

namespace Things::Decos {
/**
 * @description: 事件上报功能类
 */
class EventEmitter: public Base {
    friend outer_t;
    EventEmitter(outer_t* outer);
    virtual void init() override;

private:
    Observable<bool> inited = false;
};
}
