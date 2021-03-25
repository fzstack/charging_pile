#pragma once

#include "colors.hxx"
#include <vector>
#include <memory>
#include <array>

class Buffer {
public:
    virtual Colors::Argb& get(int x, int y) = 0;
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;
    void fill(Colors::Argb color = Colors::Argb::kTransparent);
    void fill(int x, int y, int width, int height, Colors::Argb color = Colors::Argb::kTransparent);
    void blend(int selfX, int selfY, int otherX, int otherY, std::shared_ptr<Buffer> other, int width, int height);
};

class VSizeBuffer: public Buffer {
public:
    VSizeBuffer(int width, int height);
    virtual Colors::Argb& get(int x, int y) override;
    virtual int getWidth() override;
    virtual int getHeight() override;

private:
    const int width, height;
    std::vector<std::vector<Colors::Argb>> buf;
};

template<int Width, int Height>
class CSizeBuffer: public Buffer {
public:
    CSizeBuffer() {}

    virtual Colors::Argb& get(int x, int y) override {
        return buf[x][y];
    }

    virtual int getWidth() override {
        return Width;
    }

    virtual int getHeight() override {
        return Height;
    }

    std::array<std::array<Colors::Argb, Height>, Width> buf;
};



