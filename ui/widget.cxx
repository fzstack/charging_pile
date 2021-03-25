#include "widget.hxx"

using namespace std;

Widget::Widget(int x, int y, int zIndex): x(x), y(y), zIndex(zIndex) {

}

int Widget::getWidth() {
    return getBuffer()->getWidth();
}

int Widget::getHeight() {
    return getBuffer()->getHeight();
}

void Widget::addTo(std::shared_ptr<Core> core) {
    this->core = core;
}

void Widget::invalid() {
    core->invalidWidget(shared_from_this());
}
