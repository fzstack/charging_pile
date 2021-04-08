#ifndef APPLICATIONS_CLOUD_HXX_
#define APPLICATIONS_CLOUD_HXX_

#include <memory>
#include <string_view>
#include <config/bsp.hxx>
#include "cloud_timer.hxx"
#include <utilities/signals.hxx>
#include <config/app.hxx>
#include <utilities/nat_port.hxx>

class CloudTimer;

class Cloud {
    friend class CloudTimer;
public:
    Cloud(std::shared_ptr<CloudTimer> timer);
    virtual void init();

    virtual void setIccid(std::string_view iccid) = 0;

    virtual void emitHeartbeat(Heartbeat&& heartbeat) = 0;
    virtual void emitCurrentData(CurrentData&& data) = 0;
    virtual void emitPortAccess(NatPort port) = 0;
    virtual void emitPortUnplug(NatPort port) = 0;
    virtual void emitIcNumber(NatPort port, std::string_view icNumber) = 0;
    virtual void emitCurrentLimit(NatPort port) = 0;

    Signals<void()> onQuery = {};
    Signals<void()> onReboot = {};

    struct ServiceResult {
        enum Value {
            Succeed = 1, Failed,
        };
    };

    Signals<ServiceResult::Value(NatPort port, int timerId, int minutes)> onControl = {};
    Signals<ServiceResult::Value(NatPort port, int timerId)> onStop = {};
    Signals<void(DevConfig conf)> onConfig = {};
    Signals<DevConfig()> onReadConfig = {};

protected:
    virtual void setSignalInterval() = 0;

private:
    std::shared_ptr<CloudTimer> timer;
};


#endif /* APPLICATIONS_CLOUD_HXX_ */
