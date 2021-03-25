#include "deliver_base.hxx"
#include "post.hxx"
#include <rtthread.h>
#include <stdexcept>
#include <algorithm>

using namespace std;

DeliverBase::DeliverBase(outer_t* outer): base_t(outer) {

}

rt_thread_t DeliverBase::getPollingThread() {
    return outer->pollingThread;
}

void DeliverBase::dispose() {
    outer->delivers.remove(shared_from_this());
}

void DeliverBase::enqueue() {
    outer->queue.push(shared_from_this());
}

