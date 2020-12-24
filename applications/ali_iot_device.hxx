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
#include <json.hxx>
#include <utilities/signals.hxx>

class AliIotDevice {
public:
    AliIotDevice(std::shared_ptr<HttpClient> http, std::shared_ptr<MqttClient> mqtt);
    void login(std::string_view deviceName, std::string_view productKey, std::string_view deviceSecret);

    void on(std::string_view serviceName, std::function<Json(Json data)> handler);

private:
    static std::string getSign(std::string_view deviceName, std::string_view productKey, std::string_view deviceSecret);
    static std::string hexToAscii(const std::vector<char>& hex);
    static std::vector<std::string> splitTopic(std::string_view topic);

private:
    std::shared_ptr<HttpClient> http;
    std::shared_ptr<MqttClient> mqtt;

    //std::map<std::string, Signals<std::function<Json(Json data)>>> services = {};

    static const char* kApiAuth;

};

class ali_iot_error: public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class ali_iot_auth_failed: public ali_iot_error {
    using ali_iot_error::ali_iot_error;
};



#endif /* APPLICATIONS_ALI_IOT_DEVICE_HXX_ */
