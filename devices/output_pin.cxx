#include "output_pin.hxx"
#include <rtdevice.h>
#include <functional>

OutputPin::OutputPin(rt_base_t pin): pin(pin) {
    inited.onChanged += [=](auto value) {
      if(value) {
          rt_pin_mode(pin, PIN_MODE_OUTPUT);
      } else {
          rt_pin_mode(pin, PIN_MODE_INPUT);
      }
    };

    handler.onChanged += [=](auto value) {
        if(!value) {
            inited = false;
            return;
        }
        inited = true;
        rt_pin_write(pin, *value ? PIN_HIGH : PIN_LOW);
    };
}

OutputPin::handler_t& OutputPin::getHandler() {
    return handler;
}

OutputPin::operator handler_t&() {
    return getHandler();
}

OutputPin::handler_t& OutputPin::operator*() {
    return getHandler();
}
