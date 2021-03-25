#ifdef TEST_PACKET

#include <components/packet.hxx>
#include <rtthread.h>
#include <utilities/cmd.hxx>
#include <string>

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

    return RT_EOK;
}

INIT_APP_EXPORT(init_test_packet);
MSH_CMD_EXPORT(test_packet_bool, );
MSH_CMD_EXPORT(test_packet_opt, );
MSH_CMD_EXPORT(test_packet_scores, );
MSH_CMD_EXPORT(test_packet_map, );
MSH_CMD_EXPORT(test_packet_array, );
#endif
