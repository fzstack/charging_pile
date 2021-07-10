#include "ali_iot_device.hxx"
#include <rtthread.h>
#include <tinycrypt.h>
#include <string_view>
#include <components/http_form_request_builder.hxx>
#include <components/http_request.hxx>
#include <utilities/json.hxx>
#include <utilities/string.hxx>
#include <variant>
#include "ali_iot_alink.hxx"
#include <components/persistent_storage_preset.hxx>


#define LOG_TAG "app.ali_iot"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;
using namespace rtthread;
using namespace string_literals;
using namespace json_literals;

AliIotDevice::AliIotDevice(std::shared_ptr<CommDev> commDev, shared_ptr<HttpClient> http, shared_ptr<MqttClient> mqtt, std::shared_ptr<SharedThread> thread):
  commDev(commDev),
  http(http),
  mqtt(mqtt),
  thread(thread),
  oConnected(mqtt->oConnected) {
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
                    rt_kprintf("\033[31mprop [%s] invoke failed: %s\n\033[0m", name.c_str(), e.what());
                }
            }, value);
        }
        return {};
    });

    mqtt->onMessage += this->thread->post([this](string topic, string data) {
        rt_kprintf("topic: %s\ndata: %s\n", topic.c_str(), data.c_str());
        auto topics = split(topic, '/');
        if(topics[1] == "ota" && topics[2] == "device" && topics[3] == "upgrade") {
            auto reply = Alink::Reply::from(data);
            const auto d = reply.getData();
            rt_kprintf("\nOTA UPGRADE, NEW VERSION: %s\n", d["version"_s].c_str());
            rt_kprintf("creating stream...\n");
            auto stream = this->http->stream(
                std::make_shared<HttpRequest>()
                ->setUrl(d["url"_s])
                ->setMethod(HttpMethod::Get));
            rt_kprintf("invoking ota cb...\n");
            auto module = "default"s;
            if(d.contains("module")) {
                module = d["module"_s];
            }
            ota(d["version"_s], module, stream, d["size"_i]);
            rt_kprintf("ota cb returned\n");
            return;
        }

        auto request = Alink::Request::from(data);
        auto methods = request.getMethod();

        auto action = unordered_map<string, function<void()>> {
            {"thing", [&](){
                auto action = unordered_map<string, function<void()>> {
                    {"service", [&]() {
                        auto identifier = topics[TopicIdx::Thing::Service::Identifier];
                        auto service = services.find(identifier);
                        if(service == services.end()) return;
                        auto id = request.getId();
                        service->second(this->thread->post([this, identifier, topic, id](variant<Json, exception_ptr> result){
                            auto json = get_if<Json>(&result);
                            try {
                                if(auto err = get_if<exception_ptr>(&result)) std::rethrow_exception(*err);
                            } catch (const exception& e) {
                                rt_kprintf("\033[31mservice [%s] async invoke failed: %s\n\033[0m", identifier.c_str(), e.what());
                                return;
                            }
                            auto reply = Alink::Reply(*json, 200, id);
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
                auto id = request.getId();
                service->second(this->thread->post([this, requestId, serviceName, id](variant<Json, exception_ptr> result) {
                    auto json = get_if<Json>(&result);
                    try {
                        if(auto err = get_if<exception_ptr>(&result)) std::rethrow_exception(*err);
                    } catch (const exception& e) {
                        rt_kprintf("\033[31mservice [%s] sync invoke failed: %s\n\033[0m", serviceName.c_str(), e.what());
                        return;
                    }
                    auto reply = Alink::Reply(*json, 200, id);
                    auto topic = genTopic({"rrpc", "response", requestId});
                    this->mqtt->publish(topic, (string)reply);
                }), request.getParams());
            }},
        };
        auto found = action.find(topics[TopicIdx::ThingOrRrpc]);
        if(found != action.end()) found->second();
    });
    //thread->start();

    oConnected += [=](auto value) {
        if(!value) {
            thread->exec([=] {
                rt_kprintf("relogin due to disconnected\n");
                loginInternal();
            });
        }
    };
}

//TODO: Signal 转 function
//TODO: 无返回值的Post无参数返回值回调
void AliIotDevice::login(string_view deviceName, string_view productKey, string_view productSecret) {
    this->deviceName = deviceName;
    rt_kprintf("device name: from %s to %s\n", deviceName.data(), this->deviceName.data());
    this->productKey = productKey;
    this->productSecret = productSecret;
    
    loginInternal();
}

void AliIotDevice::loginInternal() {
    rt_kprintf("initing ali cloud\n");
    commDev->reinit();
    rt_kprintf("enter login...\n");
    //自动注册
    auto storage = Preset::PersistentStorage::get();
    auto conf = storage->read<Config::AliIotDevice>();
    if(deviceName != conf.deviceName || productKey != conf.productKey) {
        auto random = 1234;
        auto sign = getRegisterSign(deviceName, productKey, productSecret, random);
        rt_kprintf("device name: %s\n", deviceName.data());
        auto json = Json::parse(http->send(
            HttpFormRequestBuilder{}
            .setUrl(kApiRegister)
            .addParam("productKey", productKey)
            .addParam("deviceName", deviceName)
            .addParam("random", to_string(random))
            .addParam("sign", sign)
            .addParam("signMethod", "HmacMD5")
            .build()
        ));

        auto code = json["code"_i];
        if(code != 200) throw ali_iot_auth_failed{"device auth failed with code "s + to_string(code)};

        conf.deviceName = deviceName;
        conf.productKey = productKey;
        conf.deviceSecret = json["data"]["deviceSecret"_s];
        storage->write(conf);
        rt_kprintf("reg succeed\n");
    }

    auto sign = getLoginSign(deviceName, productKey, conf.deviceSecret);

    rt_kprintf("try send http req...\n");
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
}

void AliIotDevice::emit(std::string_view event, Json params) {
    auto request = string(Alink::Request(params, "thing.event."s + event.data() + ".post"));
    mqtt->publish(genTopic({"thing", "event", event, "post"}), request);
}

void AliIotDevice::set(std::string_view property, Json value) {
    emit("property", {
       {property, value},
    });
}

void AliIotDevice::log(std::string_view msg) {
    auto request = string(Alink::Request(Json::array({{
        {"utcTime", ""},
        {"logLevel", "INFO"},
        {"module", "pre"},
        {"code", "4103"},
        {"logContent", msg},
    }}), "thing.log.post"));
    mqtt->publish(genTopic({"thing", "log", "post"}), request);
}

void AliIotDevice::otaEmitVersion(std::string_view version, std::string_view module) {
    auto request = string(Alink::Request({
       {"version", version},
       {"module", module},
    }, ""));
    mqtt->publish(genTopic({"ota", "device", "inform"}, {}), request);
}

void AliIotDevice::otaEmitProgress(int step, std::string_view desc, std::string_view module) {
    auto request = string(Alink::Request({
       {"step", to_string(step)},
       {"desc", desc},
       {"module", module}
    }, ""));
    mqtt->publish(genTopic({"ota", "device", "progress"}, {}), request);
}

string AliIotDevice::genTopic(initializer_list<string_view> suffixes) {
    return genTopic({"sys"}, suffixes);
}

std::string AliIotDevice::genTopic(std::initializer_list<std::string_view> prefixes, std::initializer_list<std::string_view> suffixes) {
    auto result = ""s;
    for(const auto& prefix: prefixes)
        result += '/' + string(prefix.data());
    for(const auto& e: {productKey, deviceName})
        result += '/' + e;
    for(const auto& suffix: suffixes)
        result += '/' + string(suffix.data());
    return result;
}

std::string AliIotDevice::getRegisterSign(std::string_view deviceName, std::string_view productKey, std::string_view productSecret, int random) {
    auto input = "deviceName"s + deviceName.data() + "productKey"s + productKey.data() + "random"s + to_string(random);
    return getSign(productSecret, input);
}

string AliIotDevice::getLoginSign(string_view deviceName, string_view productKey, string_view deviceSecret) {
    auto input = "clientId"s + deviceName.data() + "deviceName"s + deviceName.data() + "productKey"s + productKey.data();
    return getSign(deviceSecret, input);
}

string AliIotDevice::getSign(string_view secret, string_view content) {
    auto output = vector<char>(16);
    tiny_md5_hmac((rt_uint8_t*)secret.data(), secret.size(), (rt_uint8_t*)content.data(), content.size(), (rt_uint8_t*)&output[0]);
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
const char* AliIotDevice::kApiRegister = "https://iot-auth.cn-shanghai.aliyuncs.com/auth/register/device";
