#ifndef APPLICATIONS_ALI_CLOUD_HXX_
#define APPLICATIONS_ALI_CLOUD_HXX_

#include "cloud.hxx"
#include "ali_iot_device.hxx"
#include <devices/air724.hxx>
#include <memory>
#include <components/timer.hxx>
#include <utilities/count_down.hxx>
#include <config/bsp.hxx>
#include <array>
#include <utilities/inner_port.hxx>
#include <components/app_state.hxx>

class AliCloud: public Cloud {
public:
    AliCloud(std::shared_ptr<AliIotDevice> device, std::shared_ptr<Air724> air, std::shared_ptr<CloudTimer> timer, std::shared_ptr<AppState> appState);
    virtual void init() override;
    virtual void emitCurrentData(CurrentData&& data) override;

    //下面两个函数是云类内部调用的
    virtual void setIccid(std::string_view iccid) override;
    virtual void emitHeartbeat(Heartbeat&& heartbeat) override;

    virtual void emitPortAccess(NatPort port) override;
    virtual void emitPortUnplug(NatPort port) override;
    virtual void emitIcNumber(NatPort port, std::string_view icNumber) override;
    virtual void emitCurrentLimit(NatPort port) override;
    virtual void emitModuleVersion(std::string_view version, std::string_view module) override;
    virtual void emitOtaProgress(int step, std::string_view desc, std::string_view module) override;

    template<class... Args>
    auto post(Args&&... args) {
        return device->thread->post(std::forward<Args>(args)...);
    }

private:
    struct Spec {
        CountDown<> fPlugged = {};
        CountDown<> fUnpluged = {};
    };

    std::array<Spec, Config::Bsp::kPortNum> specs;
    Timer psTimer = {97, "acp"};
    rt_uint8_t psCnt = 0;
    bool initDone = false;
    static constexpr rt_uint8_t kPsActionsNum = 2;

    std::shared_ptr<AliIotDevice> device;
    std::shared_ptr<Air724> air;
    std::shared_ptr<AppState> appState;
    Observable<bool> inited = {false};
    rt_tick_t lastSetTick = 0;
};

#include <utilities/singleton.hxx>
namespace Preset {
class AliCloud: public Singleton<AliCloud>, public ::AliCloud {
    friend class Singleton<AliCloud>;
    AliCloud(): ::AliCloud(AliIotDevice::get(), Air724::get(), CloudTimer::get(this), AppState::get()) {};
};
}

#endif /* APPLICATIONS_ALI_CLOUD_HXX_ */
