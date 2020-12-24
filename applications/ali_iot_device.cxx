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
#include "json.hxx"
#include <string_view>
#include <components/http_form_request_builder.hxx>

#define LOG_TAG "app.ali_iot"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;
using namespace string_literals;
using namespace json_literals;

AliIotDevice::AliIotDevice(shared_ptr<HttpClient> http, shared_ptr<MqttClient> mqtt): http(http), mqtt(mqtt) {

}

void AliIotDevice::login(string_view deviceName, string_view productKey, string_view deviceSecret) {
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

    //TODO: run in process thread
    mqtt->onMessage += [this](auto topic, auto data) {

        rt_kprintf("topic: %s\ndata: %s\n", topic.c_str(), data.c_str());
        rt_kprintf("topic levels: \n");

        auto topics = splitTopic(topic);
//        for(const auto& t: topics) {
//            rt_kprintf("%s\n", t.c_str());
//        }
//
//        if(topics[3] == "rrpc") {
//            auto reqId = topics[5];
//        }

//        auto json = Json::parse(data);
//        rt_kprintf("method: %s\n", to_string(json).c_str());


    };

    mqtt->login(productKey.data() + ".iot-as-mqtt.cn-shanghai.aliyuncs.com"s, deviceName, iotId, iotToken);
    LOG_I("login succeed");
}

void AliIotDevice::on(string_view serviceName, function<Json(Json data)> handler) {
//    auto found = services.find(serviceName);
//    if(found == services.end()) {
//        //mqtt->sub TODO: 订阅
//    }
//    services[serviceName] += handler;
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

vector<string> AliIotDevice::splitTopic(string_view topic) {
    size_t pos = 0, nextPos = 0;
    auto result = vector<string>{};
    topic.find('/', pos);

    while((nextPos = topic.find('/', pos + 1)) != string::npos) {
        result.push_back((string)topic.substr(pos + 1, nextPos - pos - 1));
        pos = nextPos;
    }
    result.push_back((string)topic.substr(pos + 1, topic.size() - pos - 1));
    return result;
}

const char* AliIotDevice::kApiAuth = "https://iot-auth.cn-shanghai.aliyuncs.com/auth/devicename";

