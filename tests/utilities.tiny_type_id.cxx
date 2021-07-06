#include <utilities/tiny_type_id.hxx>
#include <rtthread.h>

template<class T>
struct Wrapper{

};

void test_tiny_type_id() {
    rt_kprintf("hash of int: %08x\n", TypeId<int>::get());
    rt_kprintf("hash of long: %08x\n", TypeId<long>::get());
    rt_kprintf("hash of Wrapper<int>: %08x\n", TypeId<Wrapper<int>>::get());
    rt_kprintf("hash of Wrapper<long>: %08x\n", TypeId<Wrapper<long>>::get());
}

MSH_CMD_EXPORT(test_tiny_type_id, );

