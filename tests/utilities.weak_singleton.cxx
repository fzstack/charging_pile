#include <utilities/weak_singleton.hxx>
#include <rtthread.h>
#include <utilities/cmd.hxx>

class WeakSingletonTest {
public:
    WeakSingletonTest(char* name): name(name) {
        rt_kprintf("%s created\n", name);
    }

    void fn() {
        rt_kprintf("%s fn called\n", name);
    }

    ~WeakSingletonTest() {
        rt_kprintf("%s destroyed\n", name);
    }

private:
    char* name;
};

class WeakSingletonTestWeak: public WeakSingletonTest, public WeakSingleton<WeakSingletonTestWeak> {
public:
    WeakSingletonTestWeak(): WeakSingletonTest("weak") {}
};

class WeakSingletonTestHold: public WeakSingletonTest, public WeakSingleton<WeakSingletonTestHold> {
public:
    WeakSingletonTestHold(): WeakSingletonTest("hold") {}
};



static std::shared_ptr<WeakSingletonTestHold> hold = nullptr;

static void weakSingleton(int argc, char** argv) {
    Cmd{argc, argv}([](Cmd& cmd) {
        cmd.select({
        {"weak", [&]{
            cmd.select({{"fn", [&]{
                WeakSingletonTestWeak::get()->fn();
            }}});
        }},
        {"hold", [&]{
            cmd.select({
            {"create", [&]{
                hold = WeakSingletonTestHold::get();
            }},
            {"fn", [&]{
                hold->fn();
            }},
            {"destory", [&]{
                hold = nullptr;
            }},
            });
        }},
        });
    });
}

MSH_CMD_EXPORT(weakSingleton, );