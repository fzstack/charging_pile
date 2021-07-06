#include <components/packet.hxx>
#include <rtthread.h>
#include <utilities/cmd.hxx>
#include <string>
#include <utilities/shared_thread.hxx>
#include <utilities/mp.hxx>
#include <cstdlib>

#define LOG_TAG "test.packet"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;


namespace Packets {

struct TestBool {
    bool v;
};

struct TestScores {
    struct Score {
        std::string name;
        int score;
    };
    std::vector<Score> scores;
};

struct TestOpt {
    std::optional<std::string> value;
};

struct TestMap {
    std::map<std::string, int> scores;
};

struct TestArray {
    std::array<int, 2> values;
};

template<int N>
struct TestMess {
    vector<rt_uint8_t> data;
};

}

static void test_packet_bool(int argc, char** argv) {
    auto packet = Preset::Packet::get();
    packet->emit<Packets::TestBool>({true});
}

static void test_packet_opt(int argc, char** argv) {
    auto packet = Preset::Packet::get();
    if(argc > 1) {
        packet->emit<Packets::TestOpt>({argv[1]});
    } else {
        packet->emit<Packets::TestOpt>({{}});
    }
}

static void test_packet_scores(int argc, char** argv) {
    auto packet = Preset::Packet::get();
    packet->emit<Packets::TestScores>({{
        {"Lisa", 98},
        {"Tom", 72},
        {"Lily", 84},
    }});
}

static void test_packet_map(int argc, char** argv) {
    auto packet = Preset::Packet::get();
    packet->emit<Packets::TestMap>({{
        {"Lisa", 98},
        {"Tom", 72},
        {"Lily", 84},
    }});
}

static void test_packet_array(int argc, char** argv) {
    ASSERT_MIN_NARGS(3);
    auto packet = Preset::Packet::get();
    packet->emit<Packets::TestArray>({{atoi(argv[1]), atoi(argv[2])}});
}

static constexpr int kThrCnt = 5, kDataLen = 1024;

struct Mess {
    std::shared_ptr<SharedThread> thread = std::make_shared<SharedThread>(1024, 22, 1);
};

auto messes = array<Mess, kThrCnt>{};

static void test_packet_mess() {
    auto packet = Preset::Packet::get();
    for(auto i = 0u; i < messes.size(); i++) {
        messes[i].thread->exec([=]{
            for(auto j = 0; j < 100; j++) {

                auto data = vector<rt_uint8_t>{};
                auto len = rand() % kDataLen;
                for(auto k = 0; k < len; k++) {
                    data.push_back((rt_uint8_t)rand());
                }

                rt_thread_mdelay(rand() % 1000);

                rt_kprintf("[======================================================t%d======================================================]\n", i);
                magic_switch<kThrCnt>{}([&](auto v) {
                    packet->emit<Packets::TestMess<decltype(v)::value>>({std::move(data)});
                }, i);
            }
        });
    }
}

static int init_test_packet() {
    auto packet = Preset::Packet::get();

    packet->on<Packets::TestBool>([](auto p) {
        rt_kprintf("bool: %d\n", p->v);
    });

    packet->on<Packets::TestScores>([](auto p) {
        rt_kprintf("scores: ");
        for(const auto& s: p->scores) {
            rt_kprintf("%s: %d, ", s.name.c_str(), s.score);
        }
        rt_kprintf("\n");
    });

    packet->on<Packets::TestOpt>([](auto p) {
        if(p->value.has_value()) {
            rt_kprintf("opt: %s\n", p->value->c_str());
        } else {
            rt_kprintf("opt: <empty>\n");
        }
    });

    packet->on<Packets::TestMap>([](auto p) {
        rt_kprintf("scores: ");
        for(const auto& [name, score]: p->scores) {
            rt_kprintf("%s: %d, ", name.c_str(), score);
        }
        rt_kprintf("\n");
    });

    packet->on<Packets::TestArray>([](auto p) {
        rt_kprintf("values: ");
        for(const auto& e: p->values) {
            rt_kprintf("%d, ", e);
        }
        rt_kprintf("\n");
    });

    for(auto i = 0; i < kThrCnt; i++) {
        magic_switch<kThrCnt>{}([&](auto v){
            packet->on<Packets::TestMess<decltype(v)::value>>([=](auto p) {
                //DO NOTHING
                messes[0].thread->exec([=]{
                    rt_thread_mdelay(100);
                    rt_kprintf("[%08x]{%d} %d\n", rt_tick_get(), i, p->data.size());
                });

            });
        }, i);
    }



    for(auto& mess: messes) {
        mess.thread->start();
    }

    return RT_EOK;
}

INIT_APP_EXPORT(init_test_packet);
MSH_CMD_EXPORT(test_packet_bool, );
MSH_CMD_EXPORT(test_packet_opt, );
MSH_CMD_EXPORT(test_packet_scores, );
MSH_CMD_EXPORT(test_packet_map, );
MSH_CMD_EXPORT(test_packet_array, );
MSH_CMD_EXPORT(test_packet_mess, );
