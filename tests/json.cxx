/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-20     imgcr       the first version
 */

#include "json.hxx"
#include <rtthread.h>
#include <stdexcept>

#define LOG_TAG "test.json"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

#ifdef TEST_JSON
static void test_json_dyn_create() {
    try {
        auto json = Json{};
        auto f = json["test"];
        LOG_I("create test field succeed, %s", to_string(json).c_str());

        const auto cjson = Json{};
        auto cf = cjson["ctest"];
        LOG_I("create ctest field succeed, %s", to_string(json).c_str());

    } catch(const exception& e) {
        LOG_E("error: %s", e.what());
    }
}

static void test_json_dyn_type() {
    try {
        auto json = Json{};
        json["test"]["dyn"]["type"] = 233;
        LOG_I("%s", to_string(json).c_str());
        json["test"] = 233; //obj -> int
        LOG_I("%s", to_string(json).c_str());
        json["test"]["json"] = "hello"; //int -> obj
        LOG_I("%s", to_string(json).c_str());
        json["test"] = "json"; //obj -> str
        LOG_I("%s", to_string(json).c_str());
        json["test"]["what"] = "world"; //str -> obj
        LOG_I("%s", to_string(json).c_str());
    } catch(const exception& e) {
        LOG_E("error: %s", e.what());
    }
}

static void test_json_field_fork() {
    try {
        auto json = Json{};
        auto types = json["types"];
        types["bool"]["true"] = true;
        types["bool"]["false"] = false;
        types["null"] = nullptr;
        types["int"] = 234;
        types["string"] = "hello";
        auto obj = types["obj"];
        obj["some"]["field"] = "test";
        rt_kprintf("%s\n", to_string(json).c_str());
    } catch(const exception& e) {
        LOG_E("error: %s", e.what());
    }
}

static void test_json_move_assign() {
    rt_uint32_t used;
    rt_memory_info(RT_NULL, &used, RT_NULL);
    LOG_W("used mem: %d", used);
    {
        LOG_I("moving sJson to tJson (root to root)");
        auto tJson = Json{};
        tJson["code"] = 200;
        LOG_D("origin tJson: %s", to_string(tJson).c_str());
        auto sJson = Json{};
        sJson["user"] = "hinker";
        sJson["pwd"] = "233";
        LOG_D("origin sJson: %s", to_string(sJson).c_str());
        tJson = move(sJson);
        LOG_D("tJson: %s", to_string(tJson).c_str());
        LOG_D("sJson: %s", to_string(sJson).c_str());
    }
    rt_thread_mdelay(1);
    {
        LOG_I("moving sJson to tJson.data (to last child)");
        auto tJson = Json{};
        tJson["code"] = 200;
        LOG_D("origin tJson: %s", to_string(tJson).c_str());
        auto sJson = Json{};
        sJson["user"] = "hinker";
        sJson["pwd"] = "233";
        LOG_D("origin sJson: %s", to_string(sJson).c_str());
        tJson["data"] = move(sJson);
        LOG_D("tJson: %s", to_string(tJson).c_str());
        LOG_D("sJson: %s", to_string(sJson).c_str());
    }
    rt_thread_mdelay(1);
    {
        LOG_I("moving sJson to tJson.data (to inner child)");
        auto tJson = Json{};
        tJson["code"] = 200;
        tJson["data"] = nullptr;
        tJson["extra"] = "field";
        LOG_D("origin tJson: %s", to_string(tJson).c_str());
        auto sJson = Json{};
        sJson["user"] = "hinker";
        sJson["pwd"] = "233";
        LOG_D("origin sJson: %s", to_string(sJson).c_str());
        tJson["data"] = move(sJson);
        LOG_D("tJson: %s", to_string(tJson).c_str());
        LOG_D("sJson: %s", to_string(sJson).c_str());
    }
    rt_thread_mdelay(1);
    {
        LOG_I("moving sJson to tJson.code (to first child)");
        auto tJson = Json{};
        tJson["code"] = 200;
        tJson["data"] = nullptr;
        tJson["extra"] = "field";
        LOG_D("origin tJson: %s", to_string(tJson).c_str());
        auto sJson = Json{};
        sJson["user"] = "hinker";
        sJson["pwd"] = "233";
        LOG_D("origin sJson: %s", to_string(sJson).c_str());
        tJson["code"] = move(sJson);
        LOG_D("tJson: %s", to_string(tJson).c_str());
        LOG_D("sJson: %s", to_string(sJson).c_str());
    }
    rt_thread_mdelay(1);
    {
        LOG_I("moving sJson.old_data to tJson.code(from first)");
        auto tJson = Json{};
        tJson["code"] = 200;
        LOG_D("origin tJson: %s", to_string(tJson).c_str());
        auto sJson = Json{};
        auto oldData = sJson["old_data"];
        oldData["user"] = "hinker";
        oldData["pwd"] = "233";
        LOG_D("origin sJson: %s", to_string(sJson).c_str());
        tJson["data"] = move(sJson["old_data"]);
        LOG_D("tJson: %s", to_string(tJson).c_str());
        LOG_D("sJson: %s", to_string(sJson).c_str());
    }
    rt_thread_mdelay(1);
    {
        LOG_I("moving sJson.old_data.sex to tJson.data(from inner)");
        auto tJson = Json{};
        tJson["code"] = 200;
        LOG_D("origin tJson: %s", to_string(tJson).c_str());
        auto sJson = Json{};
        auto oldData = sJson["old_data"];
        oldData["user"] = "hinker";
        oldData["sex"] = "male";
        oldData["pwd"] = "233";
        LOG_D("origin sJson: %s", to_string(sJson).c_str());
        tJson["data"] = move(sJson["old_data"]["sex"]);
        LOG_D("tJson: %s", to_string(tJson).c_str());
        LOG_D("sJson: %s", to_string(sJson).c_str());
    }
    rt_thread_mdelay(1);
    {
        LOG_I("moving sJson.old_data.pwd to tJson.data(from last)");
        auto tJson = Json{};
        tJson["code"] = 200;
        LOG_D("origin tJson: %s", to_string(tJson).c_str());
        auto sJson = Json{};
        auto oldData = sJson["old_data"];
        oldData["user"] = "hinker";
        oldData["sex"] = "male";
        oldData["pwd"] = "233";
        LOG_D("origin sJson: %s", to_string(sJson).c_str());
        tJson["data"] = move(sJson["old_data"]["pwd"]);
        LOG_D("tJson: %s", to_string(tJson).c_str());
        LOG_D("sJson: %s", to_string(sJson).c_str());
    }
    rt_thread_mdelay(1);
    {
        LOG_I("moving sJson.old_data to tJson(child to root)");
        auto tJson = Json{};
        tJson["code"] = 200;
        LOG_D("origin tJson: %s", to_string(tJson).c_str());
        auto sJson = Json{};
        auto oldData = sJson["old_data"];
        oldData["user"] = "hinker";
        oldData["pwd"] = "233";
        LOG_D("origin sJson: %s", to_string(sJson).c_str());
        tJson = move(sJson["old_data"]);
        LOG_D("tJson: %s", to_string(tJson).c_str());
        LOG_D("sJson: %s", to_string(sJson).c_str());
    }


    rt_memory_info(RT_NULL, &used, RT_NULL);
    LOG_W("used mem after test: %d", used);
}

static void test_json_copy_assign() {
    rt_uint32_t used;
    rt_memory_info(RT_NULL, &used, RT_NULL);
    LOG_W("used mem: %d", used);
    try {
        auto tJson = Json{};
        {
            auto sJson = Json{};
            sJson["value"] = 233;
            LOG_D("copy ctor test");
            tJson["copy"] = sJson;
            LOG_D("sJson: %s", to_string(sJson).c_str());
        }
        LOG_D("tJson: %s", to_string(tJson).c_str());
    } catch(const exception& e) {
        LOG_E("error: %s", e.what());
    }
    rt_memory_info(RT_NULL, &used, RT_NULL);
    LOG_W("used mem: %d", used);
}

static void test_json_init_list() {
    try {
        auto json = Json {
            {"hello", "Json"},
            {"types", {
                {"int", 5},
                {"string", "foo"},
                {"bool", true},
                {"null", nullptr},
                {"obj", {
                    {"key1", "value1"},
                    {"key2", "value2"},
                }},
            }},
        };
        rt_kprintf("%s\n", to_string(json).c_str());
    } catch(const exception& e) {
        LOG_E("err: %s", e.what());
    }
}

static void test_json_parse() {
    try {
        auto json = Json::parse("{\"test\": \"hello\"}");
        auto tJson = Json();
        tJson["data"] = move(json);
        rt_kprintf("cJson %s\n", to_string(json).c_str());
        rt_kprintf("tJson %s\n", to_string(tJson).c_str());
    } catch(const exception& e) {
        LOG_E("error: %s", e.what());
    }
}

static int init_test_json() {
    return RT_EOK;
}

MSH_CMD_EXPORT(test_json_dyn_create, );
MSH_CMD_EXPORT(test_json_dyn_type, );
MSH_CMD_EXPORT(test_json_field_fork, );
MSH_CMD_EXPORT(test_json_move_assign, );
MSH_CMD_EXPORT(test_json_copy_assign, );
MSH_CMD_EXPORT(test_json_parse, );
MSH_CMD_EXPORT(test_json_init_list, );
INIT_APP_EXPORT(init_test_json);
#endif
