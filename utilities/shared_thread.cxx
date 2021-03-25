#include "shared_thread.hxx"

void SharedThread::run(void *p) {
    post.poll();
}

