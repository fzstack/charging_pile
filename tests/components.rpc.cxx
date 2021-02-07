/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-05     imgcr       the first version
 */


#include <components/rpc.hxx>
#include <utilities/cmd.hxx>
#include <optional>
#include <numeric>
#include <string>
#include <map>

#ifdef TEST_RPC

#define LOG_TAG "test.rpc"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;


struct Acc {
    vector<string> items;
    string splitter;
};

template<>
struct RpcTrait<Acc> {
    using result_t = std::string;
};

void test_rpc_acc(int argc, char** argv) {
    ASSERT_MIN_NARGS(4);
    auto splitter = argv[1];
    auto items = vector<string>(argc - 2);
    for(auto i = 2; i < argc; i++) {
        items[i - 2] = argv[i];
    }
    auto rpc = Preset::Rpc::get();
    auto r = rpc->invoke<Acc>({items, splitter});
    rt_kprintf("acc result: %s\n", r.c_str());
}

int init_test_rpc_acc() {
    auto rpc = Preset::Rpc::get();
    rpc->def<Acc>([](auto p){
        return std::accumulate(next(p->items.begin()), p->items.end(), p->items[0], [&](string a, string b) {
            return std::move(a) + p->splitter + b;
        });
    });
    return RT_EOK;
}

MSH_CMD_EXPORT(test_rpc_acc, );
INIT_APP_EXPORT(init_test_rpc_acc);


struct Throw {
    optional<string> msg;
};

void test_rpc_throw(int argc, char** argv) {
    auto rpc = Preset::Rpc::get();
    auto t = Throw{};
    if(argc > 1)
        t.msg = argv[1];
    try {
        rpc->invoke(t);
        rt_kprintf("invoke succeed\n");
    } catch(const exception& e) {
        rt_kprintf("err: %s\n", e.what());
    }
}

int init_test_rpc_throw() {
    auto rpc = Preset::Rpc::get();
    rpc->def<Throw>([](auto p){
        if(p->msg) {
            throw runtime_error{*p->msg};
        }
    });
    return RT_EOK;
}

MSH_CMD_EXPORT(test_rpc_throw, );
INIT_APP_EXPORT(init_test_rpc_throw);

struct Delay {
    int ms;
};

void test_rpc_delay(int argc, char** argv) {
    ASSERT_MIN_NARGS(2);
    auto ms = atoi(argv[1]);
    auto rpc = Preset::Rpc::get();
    auto before = rt_tick_get();
    rpc->invoke<Delay>({ms});
    rt_kprintf("elapsed: %dms\n", rt_tick_get() - before);
}

#include <utilities/shared_thread.hxx>
SharedThread testShared(1024, 15, 10, "test");

int init_test_rpc_delay() {
    auto rpc = Preset::Rpc::get();
    rpc->def<Delay>([](auto p, auto r) {
        testShared.exec([=]{
            rt_thread_mdelay(p->ms);
            r({});
        });
    });
    testShared.start();
    return RT_EOK;
}

MSH_CMD_EXPORT(test_rpc_delay, );
INIT_APP_EXPORT(init_test_rpc_delay);

enum class Gender {
    Male,
    Female,
};

enum class Course {
    Language,
    Math,
    Biology,
    Chemistry,
};

struct Record {
    int age;
    Gender gender;
    vector<string> friends;
    map<Course, float> scores;
};

struct Teacher {
    string name;
    Gender gender;
};

struct Class {
    map<string, Record> students;
    optional<Teacher> currentTeacher;
};

void test_rpc_class(int argc, char** argv) {
    auto rpc = Preset::Rpc::get();
    rpc->invoke<Class>({
        students: {
            {"Lisa", {
                age: 15,
                gender: Gender::Female,
                friends: {"Tom", "Lily"},
                scores: {
                    {Course::Language, 90},
                    {Course::Math, 80},
                }
            }},
            {"Tom", {
                age: 16,
                gender: Gender::Male,
                friends: {"Lisa", "Alan", "Todd"},
                scores: {
                    {Course::Language, 78},
                    {Course::Biology, 84},
                    {Course::Chemistry, 91},
                }
            }},
        },
        currentTeacher: Teacher {
            name: "Tyler",
            gender: Gender::Male,
        }
    });
}

static map<Gender, string> genName = {
    {Gender::Male, "male"},
    {Gender::Female, "female"},
};

static map<Course, string> courseName = {
    {Course::Language, "language"},
    {Course::Math, "math"},
    {Course::Biology, "biology"},
    {Course::Chemistry, "chemistry"},
};

int init_test_rpc_class() {
    auto rpc = Preset::Rpc::get();
    rpc->def<Class>([](auto p) {
        if(p->currentTeacher) {
            rt_kprintf("current teacher: %s\n", p->currentTeacher->name.c_str());
        } else {
            rt_kprintf("there is no teacher..\n");
        }
        rt_kprintf("students:\n");
        for(auto& [student, record]: p->students) {
            rt_kprintf(" %s age: %d, gender: %s, friends: { ", student.c_str(), record.age, genName[record.gender].c_str());
            for(auto& f: record.friends) {
                rt_kprintf("%s ", f.c_str());
            }
            rt_kprintf("} scores: { ");
            for(auto& [course, score]: record.scores) {
                rt_kprintf("%s:%d ", courseName[course].c_str(), int(score));
            }
            rt_kprintf("}\n");
        }
    });
    return RT_EOK;
}

MSH_CMD_EXPORT(test_rpc_class, );
INIT_APP_EXPORT(init_test_rpc_class);

#endif

