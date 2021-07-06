#include <rtthread.h>
#include <ui/widget.hxx>
#include <ui/ws_screen.hxx>
#include <utilities/cmd.hxx>
#include <map>

#define LOG_TAG "test.widget"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;

class TestWidget: public Widget {
public:
    TestWidget(int x, int width, int zIndex): Widget(x, 0, width, Preset::WsScreen::kHeight, zIndex) {}

    void fillWith(Colors::Argb color) {
        this->color = color;
        invalid();
    }

protected:
    virtual void onDraw(std::shared_ptr<Graphics> graphics) override {
        graphics->fillRect(color, 0, 0, width, height);
    }

    Colors::Argb color;
};

std::shared_ptr<TestWidget> widgetA, widgetB;

void test_widget(int argc, char** argv) {
    ASSERT_MIN_NARGS(6);

    auto wi = map<string, shared_ptr<TestWidget>> {
        {"a", widgetA},
        {"b", widgetB},
    };

    auto found = wi.find(argv[1]);
    ASSERT_ARG(widget, found != wi.end());

    auto a = (rt_uint8_t)atoi(argv[2]);
    auto r = (rt_uint8_t)atoi(argv[3]);
    auto g = (rt_uint8_t)atoi(argv[4]);
    auto b = (rt_uint8_t)atoi(argv[5]);
    found->second->fillWith({a, r, g, b});
}

int init_test_widget() {
    auto core = Preset::Core::get();
    widgetA = make_shared<TestWidget>(0, 4, 1);
    widgetB = make_shared<TestWidget>(1, 4, 0);
    core->add(widgetA);
    core->add(widgetB);
    return RT_EOK;
}

MSH_CMD_EXPORT(test_widget, );
INIT_APP_EXPORT(init_test_widget);
