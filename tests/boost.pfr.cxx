#ifdef TEST_PFR
#include <stdexcept>
#include <boost/pfr.hpp>
#include <memory>
#include <utilities/mp.hxx>

#define LOG_TAG "test.pfr"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;
using namespace boost;

template <class T>
struct IdxBase {
    IdxBase(int test): test(test) {}
    IdxBase(std::nullptr_t n): test(55) {};

    ~IdxBase() {

    }

    void print() {
        rt_kprintf("%d\n", test);
    }

    struct InnerClass {

    };

public:
    int test = 233;
};

template<class T>
struct Idx: public IdxBase<T> {
public:
    using IdxBase<T>::IdxBase;


};

struct Node {
    static Node create() {
        auto node = Node{};
        return node;
    }

    ~Node() {

    }

    Idx<Node> next = 5;

    //Idx<Node> prev = {nullptr};
};

static void test_pfr() {
    auto value = Node();
    pfr::for_each_field(value, [&](const auto& field, std::size_t idx) {
        rt_kprintf("%d: %s = %d\n", idx, typeid(field).name(), field);

    });
}

MSH_CMD_EXPORT(test_pfr, );
#endif

