#include "event.hxx"

Event::Event() {

}

void Event::set() {
    rt_event_send(event.get(), 1);
}

void Event::wait() {
    rt_event_recv(event.get(), 1, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, nullptr);
}
