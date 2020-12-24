/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-15     imgcr       the first version
 */

#include "air_http_client.hxx"
#include <stdexcept>
#include <vector>

#define LOG_TAG "com.air_http"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

AirHttpClient::AirHttpClient(std::shared_ptr<Air724> owner): AirComponent<AirHttpClient>(owner) {
    event = shared_ptr<rt_event>(rt_event_create("air_http", RT_IPC_FLAG_FIFO), [](auto p) {
        LOG_W("close http event");
        rt_event_delete(p);
    });
}

string AirHttpClient::send(shared_ptr<HttpRequest> request) {
    auto resp = createResp();
    auto guard = shared_ptr<void>(nullptr, [this, resp](auto) {
        at_obj_exec_cmd(getAtClient(), resp.get(), "AT+HTTPTERM");
    });
    if(at_obj_exec_cmd(getAtClient(), resp.get(), "AT+HTTPINIT") != RT_EOK)
        throw runtime_error{"timeout when initing http"};
    if(at_obj_exec_cmd(getAtClient(), resp.get(), "AT+HTTPPARA=\"CID\",1") != RT_EOK)
        throw runtime_error{"timeout when setting cid"};
    if(at_obj_exec_cmd(getAtClient(), resp.get(), "AT+HTTPPARA=\"URL\",\"%s\"", request->getUrl().data()) != RT_EOK)
        throw runtime_error{"timeout when setting url"};
    for(const auto& kvp: request->getHeaders()) {
        if(at_obj_exec_cmd(getAtClient(), resp.get(), "AT+HTTPPARA=\"USER_DEFINED\",\"%s: %s\"", kvp.first.data(), kvp.second.data()) != RT_EOK)
            throw runtime_error{"timeout when addpending http headers"};
    }
    if(request->getMethod() == HttpMethod::Post) {
        auto size = request->getBody().size();
        sendBuf = &request->getBody();
        if(at_obj_exec_cmd(getAtClient(), resp.get(), "AT+HTTPDATA=%d,20000", size) != RT_EOK)
            throw runtime_error{"timeout when defining body"};
        sendBuf = nullptr;

//        auto size = request.getBody().size();
//        at_resp_set_info(resp.get(), owner->kDefaultAtRespBuffSize, 1, owner->kTimeoutMs); //消耗DOWNLOAD
//        if(at_obj_exec_cmd(getAtClient(), resp.get(), "AT+HTTPDATA=%d,20000", size) != RT_EOK)
//            throw runtime_error{"timeout when defining body"};
//        at_resp_set_info(resp.get(), owner->kDefaultAtRespBuffSize, 0, owner->kTimeoutMs);
//        if(at_obj_exec_cmd(getAtClient(), resp.get(), request.getBody().c_str()) != RT_EOK)
//            throw runtime_error{"timeout when sending data"};
    }
    if(at_obj_exec_cmd(getAtClient(), resp.get(), "AT+HTTPACTION=%d", int(request->getMethod())) != RT_EOK)
        throw runtime_error{"timeout when http action"};

    //TODO: 超时
    rt_event_recv(event.get(), Events::HttpAction, RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, RT_NULL);

    if(at_obj_exec_cmd(getAtClient(), resp.get(), "AT+HTTPREAD") != RT_EOK)
        throw runtime_error{"timeout when reading http response"};

    auto result = string(recvBuf.begin(), recvBuf.end());
    recvBuf.clear();
    return result;
}

std::vector<at_urc> AirHttpClient::onUrcTableInit() {
    LOG_D("initing http urc...");
    return {
        {"DOWNLOAD", "\r\n", [](at_client_t client, const char* data, rt_size_t size) {
            auto inst = urc(client);
            if(!inst){
                LOG_W("inst get failed");
                return;
            }

            LOG_D("on download");
            at_client_obj_send(client, &(*inst->sendBuf)[0], inst->sendBuf->size());
        }},
        {"+HTTPACTION:", "\r\n", [](at_client_t client, const char* data, rt_size_t size) {
            auto inst = urc(client);
            if(!inst) return;

            LOG_D("on http action");
            rt_event_send(inst->event.get(), Events::HttpAction);
        }},
        {"+HTTPREAD:", "\r\n", [](at_client_t client, const char* data, rt_size_t size) {
            auto inst = urc(client);
            if(!inst) return;

            LOG_D("on http read");
            int recvSize;
            sscanf(data, "+HTTPREAD: %d", &recvSize);
            vector<char> buf(recvSize);
            at_client_obj_recv(client, &buf[0], recvSize, RT_WAITING_FOREVER);
            inst->recvBuf = move(buf);
        }}
    };
}

