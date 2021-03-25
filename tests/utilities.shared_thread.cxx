#ifdef TEST_SHARED_THREAD

#include <utilities/shared_thread.hxx>
#include <rtthread.h>

void test_shared_thread() {
    auto high = Preset::SharedThread<Priority::High>::get();
    high->exec([](){
       rt_kprintf("test self is: %s\n", rt_thread_self()->name);
    });
}

MSH_CMD_EXPORT(test_shared_thread, );

#endif

