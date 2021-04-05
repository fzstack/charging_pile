#include "ws_screen.hxx"
#include <utilities/err.hxx>

WsScreen::WsScreen(std::shared_ptr<Ws2812> ws2812, int width, int height): ws2812(ws2812), width(width), height(height) {

}

int WsScreen::getWidth() {
    return width;
}

int WsScreen::getHeight() {
    return height;
}

Colors::Rgb WsScreen::getPixel(int x, int y) {
    throw not_implemented{""};
}

void WsScreen::onTick() {
    if(willFlush) {
        willFlush = false;
        this->ws2812->flush();
    }
}

void WsScreen::setPixel(Colors::Rgb color, int x, int y) {
    ws2812->getPixel(mapPixelIdx(x, y)) = color;
}

void WsScreen::flush() {
    //TODO: 定时器
    willFlush = true;
}
