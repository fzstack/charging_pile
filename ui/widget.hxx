#pragma once
#include "core.hxx"
#include "buffer.hxx"
#include "graphics.hxx"
#include <memory>

class Core;
class Buffer;

class Widget: public std::enable_shared_from_this<Widget> {
    friend class Core;
public:
    Widget(int x, int y, int zIndex);
    int getWidth();
    int getHeight();

protected:
    void invalid();
    virtual void onDraw(std::shared_ptr<Graphics> graphics) = 0;
    virtual std::shared_ptr<Buffer> getBuffer() = 0;

private:
    void addTo(std::shared_ptr<Core> core);

public:
    const int x, y, zIndex;

private:
    std::shared_ptr<Core> core = nullptr;
};

template <int Width, int Height>
class CSizeWidget: public Widget {
    using Widget::Widget;
protected:
    virtual std::shared_ptr<Buffer> getBuffer() override {
        return buffer;
    }
    std::shared_ptr<CSizeBuffer<Width, Height>> buffer = std::make_shared<CSizeBuffer<Width, Height>>();
};
