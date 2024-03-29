#include <utilities/singleton.hxx>
#include <rtthread.h>
#include <utilities/cmd.hxx>

#define LOG_TAG "test.singleton"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

class Test: public Singleton<Test> {
    friend class Singleton<Test>;
private:
    Test(int a): a(a) {}
public:
    int a;
};

void test_singleton(int argc, char** argv) {
    ASSERT_MIN_NARGS(2);
    auto value = atoi(argv[1]);
    auto inst = Test::get(value);
    rt_kprintf("value=%d, a=%d, ptr=%08x\n", value, inst->a, inst.get());
}

MSH_CMD_EXPORT(test_singleton, );
