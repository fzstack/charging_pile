#ifndef UTILITIES_EVENT_HXX_
#define UTILITIES_EVENT_HXX_

#include <rtthread.h>
#include <memory>

//TODO: event pool
class Event {
public:
    Event();
    void set();
    void wait();
private:
    std::shared_ptr<rt_event> event = std::shared_ptr<rt_event>(rt_event_create("tt", RT_IPC_FLAG_FIFO), [](auto p) {
        rt_event_delete(p);
    });
};



#endif /* UTILITIES_EVENT_HXX_ */
