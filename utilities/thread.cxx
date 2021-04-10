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

int Thread::maxUsed() {
    rt_uint8_t *ptr;
    ptr = (rt_uint8_t *)_thread->stack_addr;
    while (*ptr == '#')ptr++;
    return (_thread->stack_size - ((rt_ubase_t) ptr - (rt_ubase_t) _thread->stack_addr)) * 100 / _thread->stack_size;
}

