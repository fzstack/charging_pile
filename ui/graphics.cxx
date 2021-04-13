#include "graphics.hxx"

using namespace std;

Graphics::Graphics(std::shared_ptr<Buffer> buffer): buffer(buffer) {

}

void Graphics::fillRect(Colors::Argb color, int x, int y, int width, int height) {
    for(auto i = x; i < x + width; i++) {
        for(auto j = y; j < y + height; j++) {
            buffer->get(i, j) = color;
        }
    }
}

void Graphics::addRect(Colors::Argb color, int x, int y, int width, int height) {
    for(auto i = x; i < x + width; i++) {
        for(auto j = y; j < y + height; j++) {
            buffer->get(i, j) += color;
        }
    }
}

//包括y1在内的所有点
void Graphics::addVLine(Colors::Argb color, int x0, int y0, int y1) {
    for(auto i = min(y0, y1); i <= max(y0, y1); i++) {
        buffer->get(x0, i) += color;
    }
}

void Graphics::setPixel(Colors::Argb color, int x, int y) {
    buffer->get(x, y) = color;
}

void Graphics::addPixel(Colors::Argb color, int x, int y) {
    buffer->get(x, y) += color;
}
