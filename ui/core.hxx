#ifndef UI_CORE_HXX_
#define UI_CORE_HXX_

#include "screen.hxx"
#include "widget.hxx"
#include "buffer.hxx"
#include <memory>
#include <list>
#include <components/timer.hxx>
#include <Mutex.h>

class Widget;
class Buffer;

//每个layer都有自己的缓存
class Core: public std::enable_shared_from_this<Core> {
    friend class Widget;
public:
    Core(std::shared_ptr<Screen> screen);
    void add(std::shared_ptr<Widget> widget);

private:
    void invalidWidget(std::shared_ptr<Widget> widget);

private:
    std::shared_ptr<Screen> screen;
    std::shared_ptr<Buffer> buffer;
    std::list<std::shared_ptr<Widget>> widgets;

public:
    static constexpr int kDuration = 20;
private:
    Timer timer = {kDuration, "uicore"};
    rtthread::Mutex mutex = {"uicore"};
};

#include <utilities/singleton.hxx>
#include "ws_screen.hxx"
namespace Preset {
class Core: public Singleton<Core>, public ::Core {
    friend singleton_t;
    Core(): ::Core(WsScreen::get()) {}
};
}

#endif /* UI_CORE_HXX_ */
