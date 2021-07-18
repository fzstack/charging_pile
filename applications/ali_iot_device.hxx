#ifndef APPLICATIONS_ALI_IOT_DEVICE_HXX_

#define APPLICATIONS_ALI_IOT_DEVICE_HXX_

#include <components/http_client.hxx>
#include <components/mqtt_client.hxx>
#include <memory>
#include <string_view>
#include <string>
#include <vector>
#include <stdexcept>
#include <functional>
#include <utilities/json.hxx>
#include <utilities/signals.hxx>
#include <devices/air724.hxx>
#include <components/air_components.hxx>
#include <utilities/shared_thread.hxx>
#include <utilities/istream.hxx>
#include <components/comm_dev.hxx>
#include <utilities/observer.hxx>

class AliIotDevice {
public:
    AliIotDevice(std::shared_ptr<CommDev> commDev, std::shared_ptr<HttpClient> http, std::shared_ptr<MqttClient> mqtt, std::shared_ptr<SharedThread> thread);
    void login(std::string_view deviceName, std::string_view productKey, std::string_view productSecret);
    void emit(std::string_view event, Json params);
    void set(std::string_view property, Json value);
    void log(std::string_view msg);
    void otaEmitVersion(std::string_view version, std::string_view module);
    void otaEmitProgress(int step, std::string_view desc, std::string_view module);
private:
    void loginInternal();
    std::string genTopic(std::initializer_list<std::string_view> suffixes);
    std::string genTopic(std::initializer_list<std::string_view> prefixes, std::initializer_list<std::string_view> suffixes);
    static std::string getRegisterSign(std::string_view deviceName, std::string_view productKey, std::string_view productSecret, int random);
    static std::string getLoginSign(std::string_view deviceName, std::string_view productKey, std::string_view deviceSecret);
    static std::string getSign(std::string_view secret, std::string_view content);
    static std::string hexToAscii(const std::vector<char>& hex);

private:
    struct TopicIdx {
        enum Value {
            Sys = 1, ProductKey, DeviceName, ThingOrRrpc,
        };
        struct Thing {
            enum Value {
                Type = 5,
            };
            struct Service {
                enum Value {
                    Identifier = 6,
                };
            };
        };
        struct Rrpc {
            enum Value {
                RequestId = 6,
            };
        };
    };

    struct MethodIdx {
        struct Service {
            enum Value {
                Name = 2,
            };
        };
    };

    std::shared_ptr<CommDev> commDev;
    std::shared_ptr<HttpClient> http;
    std::shared_ptr<MqttClient> mqtt;
public:
    std::shared_ptr<SharedThread> thread;
private:
    std::string deviceName, productKey, productSecret;
    bool otaRunning = false;

public:
    std::unordered_map<std::string, Signals<Json(const Json)>> services = {};
    std::unordered_map<std::string, Signals<void(const Json)>> properties = {};
    Signals<void(std::string version, std::string module, std::string url, int size)> ota;
    Observer<bool> oConnected;

private:
    static const char* kApiAuth;
    static const char* kApiRegister;

};

class ali_iot_error: public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class ali_iot_auth_failed: public ali_iot_error {
    using ali_iot_error::ali_iot_error;
};

class ali_iot_do_not_reply: public ali_iot_error {
    using ali_iot_error::ali_iot_error;
};

#include <utilities/singleton.hxx>
#include <components/air_comm_dev.hxx>
namespace Preset {
class AliIotDevice: public Singleton<AliIotDevice>, public ::AliIotDevice {
    friend class Singleton<AliIotDevice>;
    AliIotDevice(): ::AliIotDevice(std::make_shared<AirCommDev>(Air724::get()), Air724::get()->make<HttpClient>(), Air724::get()->make<MqttClient>(), SharedThread<Priority::Middle>::get()) {}
};
}

#endif /* APPLICATIONS_ALI_IOT_DEVICE_HXX_ */
