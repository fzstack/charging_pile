#include <rtthread.h>
#include <applications/ali_cloud.hxx>
#include <utilities/cmd.hxx>
#include <utilities/f.hxx>
#include <unordered_map>
#include <functional>
#include <string>

using namespace std;

static void test_ali_cloud_init() {
    auto cloud = Preset::AliCloud::get();
    cloud->onQuery += []() {
        F{} << "@onQuery"_r << endln;
    };

    cloud->onControl += [](auto port, auto timerId, auto minutes) {
        F{} << "@onControl"_r << endln;
        F{} << "port: "_r << port.get() << endln;
        F{} << "timerId: "_r << timerId << endln;
        F{} << "minutes: "_r << minutes << endln;
        return Cloud::ServiceResult::Succeed;
    };

    cloud->onStop += [](auto port, auto timerId) {
        F{} << "@onStop"_r << endln;
        F{} << "port: "_r << port.get() << endln;
        F{} << "timerId: "_r << timerId << endln;
        return Cloud::ServiceResult::Succeed;
    };

    cloud->onConfig += [](int currentLimit, int uploadThr, int fuzedThr, int noloadCurrThr) {
        F{} << "@onConfig"_r << endln;
        F{} << "currentLimit: "_r << currentLimit << endln;
        F{} << "uploadThr: "_r << uploadThr << endln;
        F{} << "fuzedThr: "_r << fuzedThr << endln;
        F{} << "noloadCurrThr: "_r << noloadCurrThr << endln;
    };

    cloud->init();
}

static void test_ali_cloud(int argc, char** argv) {
    Cmd{argc, argv}([](Cmd& cmd){
        auto cloud = Preset::AliCloud::get();
        cmd.select<string>({
            {"emit", [&]{
                cmd.select<string>({
                    {"heartbeat", [&](){
                        cloud->emitHeartbeat(Heartbeat {
                            signal: cmd.get<int>(),
                            imeiSuff: cmd.get<string>(),
                            temperature: cmd.get<int>(),
                            humidity: cmd.get<int>(),
                            smoke: cmd.get<int>(),
                            timestamp: cmd.get<int>(),
                        });
                    }},
                    {"current_data", [&](){
                        cloud->emitCurrentData(CurrentData {
                            port: NatPort{cmd.get<rt_uint8_t>()},
                            timerId: cmd.get<int>(),
                            leftMinutes: cmd.get<int>(),
                            state: cmd.get<State::Value>(),
                            current: cmd.get<float>(),
                            voltage: cmd.get<float>(),
                            consumption: cmd.get<float>(),
                            fuse: CurrentData::Fuse::Normal,
                        });
                    }},
                    {"port_access", [&](){
                        cloud->emitPortAccess(NatPort{cmd.get<rt_uint8_t>()});
                    }},
                    {"port_unplug", [&](){
                        cloud->emitPortUnplug(NatPort{cmd.get<rt_uint8_t>()});
                    }},
                    {"ic_number", [&](){
                        cloud->emitIcNumber(NatPort{cmd.get<rt_uint8_t>()}, cmd.get<string>());
                    }},
                    {"current_limit", [&](){
                        cloud->emitCurrentLimit(NatPort{cmd.get<rt_uint8_t>()});
                    }}
                });
            }},
            {"set", [&]{
                cmd.select<string>({
                    {"iccid", [&]{
                        cloud->setIccid(cmd.get<string>());
                    }},
                });
            }},
        });
    });
}

MSH_CMD_EXPORT(test_ali_cloud_init, );
MSH_CMD_EXPORT(test_ali_cloud, );
