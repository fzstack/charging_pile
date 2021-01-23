/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-19     imgcr       the first version
 */

#include "ali_iot_device.hxx"
#include <rtthread.h>
#include <tinycrypt.h>
#include <string_view>
#include <components/http_form_request_builder.hxx>
#include <utilities/json.hxx>
#include <utilities/string.hxx>
#include <variant>
#include "ali_iot_alink.hxx"

#define LOG_TAG "app.ali_iot"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;
using namespace rtthread;
using namespace string_literals;
using namespace json_literals;

AliIotDevice::AliIotDevice(shared_ptr<HttpClient> http, shared_ptr<MqttClient> mqtt):
  http(http),
  mqtt(mqtt),
  thread(shared_ptr<AliIotDeviceThread>(new AliIotDeviceThread(this))) {

    services["property"] += thread->post([this](Json params) -> Json {
        for(const auto& kvp: params) {
            auto& [key, value] = kvp;
            auto name = (string)key;
            rt_kprintf("\033[34mprop set %s: %s\n\033[0m", name.c_str(), to_string(value).c_str());
            auto property = properties.find(name);
            if(property == properties.end()) continue;
            property->second([name](optional<exception_ptr> result){
                try {
                    if(result) std::rethrow_exception(*result);
                } catch (const exception& e) {
                    rt_kprintf("\033[31mprop [%s] invoke failed: {%s} %s\n\033[0m", name.c_str(), typeid(e).name(), e.what());
                }
            }, value);
        }
        return {};
    });

    mqtt->onMessage += thread->post([this](string topic, string data) {
        rt_kprintf("topic: %s\ndata: %s\n", topic.c_str(), data.c_str());

        auto topics = split(topic, '/');
        auto request = Alink::Request::from(data);
        auto methods = request.getMethod();

        auto action = unordered_map<string, function<void()>> {
            {"thing", [&](){
                auto action = unordered_map<string, function<void()>> {
                    {"service", [&]() {
                        auto identifier = topics[TopicIdx::Thing::Service::Identifier];
                        auto service = services.find(identifier);
                        if(service == services.end()) return;
                        service->second(thread->post([this, identifier, topic](variant<Json, exception_ptr> result){
                            auto json = get_if<Json>(&result);
                            try {
                                if(auto err = get_if<exception_ptr>(&result)) std::rethrow_exception(*err);
                            } catch (const exception& e) {
                                rt_kprintf("\033[31mservice [%s] async invoke failed: {%s} %s\n\033[0m", identifier.c_str(), typeid(e).name(), e.what());
                                return;
                            }
                            auto reply = Alink::Reply(*json);
                            this->mqtt->publish(topic + "_reply", (string)reply);
                        }), request.getParams());
                    }}
                };
                auto found = action.find(topics[TopicIdx::Thing::Type]);
                if(found != action.end()) found->second();
            }},
            {"rrpc", [&](){
                auto serviceName = methods[MethodIdx::Service::Name];
                auto requestId = topics[TopicIdx::Rrpc::RequestId];
                rt_kprintf("\033[34mservice name: %s\nrequest id: %s\n\033[0m", serviceName.c_str(), requestId.c_str());
                auto service = services.find(serviceName);
                if(service == services.end()) return;
                service->second(thread->post([this, requestId, serviceName](variant<Json, exception_ptr> result) {
                    auto json = get_if<Json>(&result);
                    try {
                        if(auto err = get_if<exception_ptr>(&result)) std::rethrow_exception(*err);
                    } catch (const exception& e) {
                        rt_kprintf("\033[31mservice [%s] sync invoke failed: {%s} %s\n\033[0m", serviceName.c_str(), typeid(e).name(), e.what());
                        return;
                    }
                    auto reply = Alink::Reply(*json);
                    auto topic = genTopic({"rrpc", "response", requestId});
                    this->mqtt->publish(topic, (string)reply);
                }), request.getParams());
            }},
        };
        auto found = action.find(topics[TopicIdx::ThingOrRrpc]);
        if(found != action.end()) found->second();
    });
    thread->start();
}

//TODO: Signal 转 function
//TODO: 无返回值的Post无参数返回值回调
void AliIotDevice::login(string_view deviceName, string_view productKey, string_view deviceSecret) {
    this->deviceName = deviceName;
    this->productKey = productKey;

    auto sign = getSign(deviceName, productKey, deviceSecret);
    auto json = Json::parse(http->send(
        HttpFormRequestBuilder{}
        .setUrl(kApiAuth)
        .addParam("productKey", productKey)
        .addParam("sign", sign)
        .addParam("clientId", deviceName)
        .addParam("deviceName", deviceName)
        .build()
    ));

    auto code = json["code"_i];
    if(code != 200) throw ali_iot_auth_failed{"device auth failed with code "s + to_string(code)};

    auto data = json["data"];
    auto iotId =  data["iotId"_s];
    auto iotToken = data["iotToken"_s];

    mqtt->login(productKey.data() + ".iot-as-mqtt.cn-shanghai.aliyuncs.com"s, deviceName, iotId, iotToken);
    LOG_I("login succeed");

    rt_uint32_t used;
    rt_memory_info(RT_NULL, &used, RT_NULL);
    rt_kprintf("\033[94login succeed, used mem: %d\n\033[0m", used);
}

void AliIotDevice::emit(std::string_view event, Json params) {
    auto request = Alink::Request(params, "thing.event."s + event.data() + ".post");
    mqtt->publish(genTopic({"thing", "event", event, "post"}), (string)request);
}

void AliIotDevice::set(std::string_view property, Json value) {
    emit("property", {
       {property, value},
    });
}

string AliIotDevice::genTopic(initializer_list<string_view> suffixes) {
    auto result = ""s;
    for(const auto& prefix: {"sys"s, productKey, deviceName})
        result += '/' + prefix;
    for(const auto& suffix: suffixes)
        result += '/' + string(suffix.data());
    return result;
}

string AliIotDevice::getSign(string_view deviceName, string_view productKey, string_view deviceSecret) {
    auto input = "clientId"s + deviceName.data() + "deviceName"s + deviceName.data() + "productKey"s + productKey.data();
    auto output = vector<char>(16);
    tiny_md5_hmac((rt_uint8_t*)deviceSecret.data(), deviceSecret.size(), (rt_uint8_t*)input.c_str(), input.size(), (rt_uint8_t*)&output[0]);
    return hexToAscii(output);
}

string AliIotDevice::hexToAscii(const vector<char>& hex) {
    auto result = string(hex.size() * 2, '\0');
    for(auto i = 0u; i < hex.size(); i++) {
        rt_sprintf((char*)result.c_str() + i * 2, "%02x", hex[i]);
    }
    return result;
}

const char* AliIotDevice::kApiAuth = "https://iot-auth.cn-shanghai.aliyuncs.com/auth/devicename";

