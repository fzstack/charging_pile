#pragma once

#include "colors.hxx"
#include "buffer.hxx"
#include <memory>

class Graphics {
public:
    Graphics(std::shared_ptr<Buffer> buffer);
    void fillRect(Colors::Argb color, int x, int y, int width, int height);
    void addRect(Colors::Argb color, int x, int y, int width, int height);
    void addVLine(Colors::Argb color, int x0, int y0, int y1);
    void setPixel(Colors::Argb color, int x, int y);
    void addPixel(Colors::Argb color, int x, int y);
private:
    std::shared_ptr<Buffer> buffer;
};


