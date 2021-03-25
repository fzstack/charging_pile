#ifndef DEVICES_OUTPUT_PIN_HXX_
#define DEVICES_OUTPUT_PIN_HXX_

#include <optional>
#include <rtthread.h>
#include <utilities/observable.hxx>

class OutputPin {
    public:
    using handler_t = Observable<std::optional<bool>>;

    explicit OutputPin(rt_base_t pin);
    handler_t& getHandler();

    operator handler_t&();
    handler_t& operator*();

private:
    rt_base_t pin;
    handler_t handler;
    Observable<bool> inited = {false};
};



#endif /* APPLICATIONS2_OUTPUT_PIN_HXX_ */
