#ifndef COMPONENTS_AIR_MQTT_CLIENT_HXX_
#define COMPONENTS_AIR_MQTT_CLIENT_HXX_

#include "mqtt_client.hxx"
#include <devices/air724.hxx>
#include <components/air_component.hxx>
#include <memory>
#include <vector>
#include <components/timer.hxx>
#include <atomic>

class Air724;

class AirMqttClient: public MqttClient, public AirComponent<AirMqttClient> {
    friend class Air724;
private:
    AirMqttClient(std::shared_ptr<Air724> owner);

public:
    virtual void login(std::string_view server, std::string_view clientId, std::string_view user, std::string_view password) override;
    virtual void subscribe(std::string_view topic) override;
    virtual void publish(std::string_view topic, std::string_view data) override;
protected:
    virtual std::vector<at_urc> onUrcTableInit() override;
private:
    std::string escape(std::string_view raw);


private:
    struct Events {
        enum Value {
            ConnectOk = 1 << 0,
            ConnackOk = 1 << 1,
            AlreadyConnect = 1 << 2,
        };
    };

    const static std::string_view
        kUrcMSubPrefix,
        kUrcMSubSuffix;


    std::shared_ptr<rt_event> event;
    Timer timer = {10000, "amc"};
};


#endif /* COMPONENTS_AIR_MQTT_CLIENT_HXX_ */
