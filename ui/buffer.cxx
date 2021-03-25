#include "buffer.hxx"
#include <algorithm>

using namespace std;

void Buffer::fill(Colors::Argb color) {
    fill(0, 0, getWidth(), getHeight(), color);
}

void Buffer::fill(int x, int y, int width, int height, Colors::Argb color) {
    for(auto i = x; i < getWidth(); i++) {
        for(auto j = y; j < getHeight(); j++) {
            get(i, j) = color;
        }
    }
}

void Buffer::blend(int selfX, int selfY, int otherX, int otherY, std::shared_ptr<Buffer> other, int width, int height) {
    for(auto dx = selfX; dx < selfX + getWidth(); dx++) {
        for(auto dy = selfY; dy < selfY + getHeight(); dy++) {
            if(dx >= otherX && dy >= otherY && dx < otherX + other->getWidth() && dy < otherY + other->getHeight()) {
                get(dx, dy) += other->get(dx - otherX, dy - otherY);
            }
        }
    }
}

VSizeBuffer::VSizeBuffer(int width, int height): width(width), height(height), buf(width) {
    for(auto i = 0; i < width; i++) {
        buf[i] = vector<Colors::Argb>(height);
    }
}

Colors::Argb& VSizeBuffer::get(int x, int y) {
    return buf[x][y];
}

int VSizeBuffer::getWidth() {
    return width;
}

int VSizeBuffer::getHeight() {
    return height;
}
