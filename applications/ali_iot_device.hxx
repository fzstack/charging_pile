/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-18     imgcr       the first version
 */
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
#include "ali_iot_device_thread.hxx"

class AliIotDeviceThread;

class AliIotDevice {
public:
    AliIotDevice(std::shared_ptr<HttpClient> http, std::shared_ptr<MqttClient> mqtt);
    void login(std::string_view deviceName, std::string_view productKey, std::string_view deviceSecret);
    void emit(std::string_view event, Json params);

private:
    std::string genTopic(std::initializer_list<std::string_view> suffixes);
    static std::string getSign(std::string_view deviceName, std::string_view productKey, std::string_view deviceSecret);
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

    std::shared_ptr<HttpClient> http;
    std::shared_ptr<MqttClient> mqtt;
    std::shared_ptr<AliIotDeviceThread> thread;

    std::string deviceName, productKey;

public:
    std::map<std::string, Signals<Json(const Json)>> services = {};
    std::map<std::string, Signals<void(const Json)>> properties = {};

private:
    static const char* kApiAuth;

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



#endif /* APPLICATIONS_ALI_IOT_DEVICE_HXX_ */
