#include "rgb_light_mini.hxx"

RgbLightMini::RgbLightMini(int i, std::shared_ptr<Ws2812> ws2812): i(i), ws2812(ws2812) {
    r.onChanged += [=](auto value) {
        if(!value) return;
        ws2812->getPixel(i).r = *value ? kMax : 0;
        ws2812->flush();
    };
    g.onChanged += [=](auto value) {
        if(!value) return;
        ws2812->getPixel(i).g = *value ? kMax : 0;
        ws2812->flush();
    };
    b.onChanged += [=](auto value) {
        if(!value) return;
        ws2812->getPixel(i).b = *value ? kMax : 0;
        ws2812->flush();
    };
}

