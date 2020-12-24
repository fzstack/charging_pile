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
#include <applications/instances.hxx>
#include <string>

#define LOG_TAG "test.air_http"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

#ifdef TEST_AIR_HTTP_CLIENT
static void test_air_http_client() {
    try {
        auto air724 = Instances::air724;
        LOG_D("create client1");
        auto client1 = air724->make<HttpClient>();
        auto request = HttpRequest{};
        request
            .setUrl("http://baidu.com/")
            .setMethod(HttpMethod::Get);
        auto resp = client1->send(request);
        LOG_I("succeed! size=%d", resp.size());
        auto show = string(resp.begin(), resp.end());
        LOG_D("data: \n%s", show.c_str());
    } catch(exception& e) {
        LOG_E("error occured: %s", e.what());
    }
}

static int init_test_air_http_client() {
    auto air724 = Instances::air724;
    air724->init();
    return RT_EOK;
}

MSH_CMD_EXPORT(test_air_http_client, );
INIT_APP_EXPORT(init_test_air_http_client);
#endif
