/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-30     imgcr       the first version
 */

#include "core.hxx"
#include <algorithm>

using namespace std;

Core::Core(std::shared_ptr<Screen> screen): screen(screen), buffer(make_shared<Buffer>(screen->getWidth(), screen->getHeight())) {

}

void Core::add(std::shared_ptr<Widget> widget) {
    widgets.push_back(widget);
    widget->addTo(shared_from_this());
    //按照z顺排序
    widgets.sort([](const auto& a, const auto& b) {
        return a->zIndex < b->zIndex;
    });
    rt_kprintf("widget count: %d\n", widgets.size());
}

void Core::invalidWidget(std::shared_ptr<Widget> widget) {
    //调用widget的onDraw函数
    widget->onDraw(make_shared<Graphics>(widget->buffer));

    //重新计算缓存
    buffer->fill(widget->x, widget->y, widget->width, widget->height, Colors::Argb::kBlack);
    for(auto wi: widgets) {
        buffer->blend(widget->x, widget->y, widget->x - wi->x, widget->y - wi->y, wi->buffer, widget->width, widget->height);
    }

    for(auto i = widget->x; i < widget->x + widget->width; i++) {
        for(auto j = widget->y; j < widget->y + widget->height; j++) {
            screen->setPixel(buffer->buf[i][j], i, j);
        }
    }
    screen->flush();
}
