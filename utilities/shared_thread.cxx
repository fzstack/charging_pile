#include "shared_thread.hxx"

void SharedThread::run(void *p) {
    post.poll();
}

void SharedThread::execInternal(std::function<void()> cb) {
    cb();
}
