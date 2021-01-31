/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-01-30     imgcr       the first version
 */
#pragma once
#include "core.hxx"
#include "buffer.hxx"
#include "graphics.hxx"
#include <memory>

class Core;

class Widget: public std::enable_shared_from_this<Widget> {
    friend class Core;
public:
    Widget(int x, int y, int width, int height, int zIndex);

protected:
    void invalid();
    virtual void onDraw(std::shared_ptr<Graphics> graphics) = 0;

private:
    void addTo(std::shared_ptr<Core> core);

public:
    const int x, y, width, height, zIndex;

private:
    std::shared_ptr<Core> core = nullptr;
    std::shared_ptr<Buffer> buffer;
};


