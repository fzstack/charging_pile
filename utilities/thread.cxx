#include "thread.hxx"
#include <stdexcept>

using namespace std;

void Thread::run(void *p) {
    try {
        onRun();
    } catch(const exception& e) {
        rt_kprintf("\033[31m[%s] %s\n\033[0m", rt_thread_self()->name, e.what());
    }

}

