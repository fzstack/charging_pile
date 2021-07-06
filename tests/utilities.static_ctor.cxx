#include <utilities/static_ctor.hxx>
#include <rtthread.h>
#include <memory>

using namespace std;

class TestStaticCtor: public StaticCtor<TestStaticCtor> {
    friend static_ctor_t;
public:
    TestStaticCtor() {
        rt_kprintf("ctor called\n");
    }

    void test() {
        rt_kprintf("test func called\n");
    }
private:
    static void staticCtor() {
        rt_kprintf("static ctor called\n");
    }
};

static void test_static_ctor() {
    auto test = make_shared<TestStaticCtor>();
    test->test();
}

MSH_CMD_EXPORT(test_static_ctor, )
