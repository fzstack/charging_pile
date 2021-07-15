#include <rtthread.h>
#include <applications/loader.hxx>
#include <applications/app.hxx>
#include <utilities/cmd.hxx>

class AppA: public App {
public:
    virtual void run() override {
        rt_kprintf("app A running\n");
    }
};

class AppB: public App {
public:
    virtual void run() override {
        rt_kprintf("app B running\n");
    }
};

class TestLoader: public Loader {
public:
     virtual std::shared_ptr<App> createApp() override {
         if(getStartMethod() == 0) {
             return std::make_shared<AppA>();
         } else {
             return std::make_shared<AppB>();
         }
     }

     void switchTo(int method) {
        restart(method);
     }
};

static auto mLoader = TestLoader{};

static int init_loader() {
    mLoader.createApp()->run();
    return RT_EOK;
}

static void loader(int argc, char** argv) {
    Cmd{argc, argv}([&](Cmd& cmd){
        cmd.select({
            {"switch", [&]{
                mLoader.switchTo(cmd.get<int>());
            }},
        });
    });
}

INIT_APP_EXPORT(init_loader);
MSH_CMD_EXPORT(loader,);