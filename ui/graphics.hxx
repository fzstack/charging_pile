#pragma once

#include "colors.hxx"
#include "buffer.hxx"
#include <memory>

class Graphics {
public:
    Graphics(std::shared_ptr<Buffer> buffer);
    void fillRect(Colors::Argb color, int x, int y, int width, int height);
    void setPixel(Colors::Argb color, int x, int y);
    void addPixel(Colors::Argb color, int x, int y);
private:
    std::shared_ptr<Buffer> buffer;
};


