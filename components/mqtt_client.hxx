#ifndef COMPONENTS_MQTT_CLIENT_HXX_
#define COMPONENTS_MQTT_CLIENT_HXX_

#include <string_view>
#include <utilities/signals.hxx>
#include <utilities/observable.hxx>
#include <utilities/observer.hxx>

class MqttClient {
public:
    virtual void login(std::string_view server, std::string_view clientId, std::string_view user, std::string_view password) = 0;
    virtual void subscribe(std::string_view topic) = 0;
    virtual void publish(std::string_view topic, std::string_view data) = 0;

protected:
    Observable<bool> connected = {false};
public:
    Observer<bool> oConnected = {connected}; 
    Signals<void(std::string topic, std::string data)> onMessage;
};

#endif /* COMPONENTS_MQTT_CLIENT_HXX_ */
