/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-15     imgcr       the first version
 */

#include "air_mqtt_client.hxx"
#include <utilities/err.hxx>
#include <cctype>
#include <utilities/string.hxx>

#define LOG_TAG "com.air_mqtt"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;
using namespace string_literals;

//TODO: 析构时关闭MQTT连接

AirMqttClient::AirMqttClient(std::shared_ptr<Air724> owner): AirComponent<AirMqttClient>(owner) {
    event = shared_ptr<rt_event>(rt_event_create("air_mqtt", RT_IPC_FLAG_FIFO), [](auto p) {
        rt_event_delete(p);
    });
}

void AirMqttClient::login(std::string_view server, std::string_view clientId, std::string_view user, std::string_view password) {
    auto resp = createResp();
    if(at_obj_exec_cmd(getAtClient(), resp.get(), "AT+MCONFIG=\"%s\",\"%s\",\"%s\"", clientId.data(), user.data(), password.data()) != RT_EOK)
        throw runtime_error{"timeout when config mqtt"};
    if(at_obj_exec_cmd(getAtClient(), resp.get(), "AT+SSLMIPSTART=\"%s\",1883", server.data()) != RT_EOK)
        throw runtime_error{"timeout when initing mqtt tcp conn"};
    auto mipStartResult = rt_uint32_t{};
    rt_event_recv(event.get(), Events::ConnectOk | Events::AlreadyConnect, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &mipStartResult);

    if(!(mipStartResult & Events::AlreadyConnect)) {
        if(at_obj_exec_cmd(getAtClient(), resp.get(), "AT+MCONNECT=1,300") != RT_EOK)
            throw runtime_error{"timeout when initing mqtt conn"};
        rt_event_recv(event.get(), Events::ConnackOk, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, RT_NULL);
    }

    onConnected();
}

void AirMqttClient::subscribe(std::string_view topic) {
    throw not_implemented{};
}

void AirMqttClient::publish(std::string_view topic, std::string_view data) {
    auto resp = createResp();
    if(at_obj_exec_cmd(getAtClient(), resp.get(), "AT+MPUB=\"%s\",0,0,\"%s\"", escape(topic).c_str(), escape(data).c_str()) != RT_EOK)
        throw runtime_error{"timeout when config mqtt"};
}

std::vector<at_urc> AirMqttClient::onUrcTableInit() {
    LOG_D("initing mqtt urc...");
    return {
        {"CONNECT OK", "\r\n", [](at_client_t client, const char* data, rt_size_t size) {
            auto inst = urc(client);
            if(!inst) return;

            LOG_D("on connect ok");
            rt_event_send(inst->event.get(), Events::ConnectOk);
        }},
        {"CONNACK OK", "\r\n", [](at_client_t client, const char* data, rt_size_t size) {
            auto inst = urc(client);
            if(!inst) return;

            LOG_D("on connack ok");
            rt_event_send(inst->event.get(), Events::ConnackOk);
        }},
        {"ALREADY CONNECT", "\r\n", [](at_client_t client, const char* data, rt_size_t size) {
            auto inst = urc(client);
            if(!inst) return;

            LOG_D("on already connect");
            rt_event_send(inst->event.get(), Events::AlreadyConnect);
        }},
        {kUrcMSubPrefix.data(), kUrcMSubSuffix.data(), [](at_client_t client, const char* data, rt_size_t size) {
            auto inst = urc(client);
            if(!inst) return;

            rt_uint32_t used;
            rt_memory_info(RT_NULL, &used, RT_NULL);
            rt_kprintf("\033[94mon SUB, used mem: %d\n\033[0m", used);

            LOG_D("on sub");
            auto buf = shared_ptr<char[]>(new char[size]);
            auto dataSize = 0;
            sscanf(data, (kUrcMSubPrefix.data() + "\"%[^\"]\",%d"s).c_str(), buf.get(), &dataSize);
            auto topic = string(buf.get());
            //LOG_E("topic is: %s", topic.c_str());
            //LOG_E("size is: %d", dataSize);

            auto result = string(dataSize, '\0');
            at_client_obj_recv(client, (char*)result.c_str(), dataSize, RT_WAITING_FOREVER);
            //LOG_E("data is: %s", result.c_str());

            inst->onMessage(move(topic), move(result));
        }},
    };
}

string AirMqttClient::escape(string_view raw) {
    auto frags = split(raw.data(), '"');
    auto result = ""s;
    for(auto i = 0u; i < frags.size() - 1; i++) {
        result += frags[i] + "\\22";
    }
    result += frags[frags.size() - 1];
    return result;
}

const string_view
    AirMqttClient::kUrcMSubPrefix = "+MSUB: ",
    AirMqttClient::kUrcMSubSuffix = " byte,";
