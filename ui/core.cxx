#include "core.hxx"
#include <algorithm>

using namespace std;

Core::Core(std::shared_ptr<Screen> screen): screen(screen), buffer(make_shared<VSizeBuffer>(screen->getWidth(), screen->getHeight())) {

}

void Core::add(std::shared_ptr<Widget> widget) {
    widgets.push_back(widget);
    widget->addTo(shared_from_this());
    //按照z顺排序
    widgets.sort([](const auto& a, const auto& b) {
        return a->zIndex < b->zIndex;
    });
}

void Core::invalidWidget(std::shared_ptr<Widget> widget) {
    //调用widget的onDraw函数
    widget->onDraw(make_shared<Graphics>(widget->getBuffer()));


    //重新计算缓存
    buffer->fill(widget->x, widget->y, widget->getWidth(), widget->getHeight(), Colors::Argb::kBlack);
    for(auto wi: widgets) {
        buffer->blend(widget->x, widget->y, wi->x, wi->y, wi->getBuffer(), widget->getWidth(), widget->getHeight());
    }

    for(auto i = widget->x; i < widget->x + widget->getWidth(); i++) {
        for(auto j = widget->y; j < widget->y + widget->getHeight(); j++) {
            screen->setPixel(buffer->get(i, j), i, j);
        }
    }
    screen->flush();
}
