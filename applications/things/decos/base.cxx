#include "base.hxx"

using namespace Things::Decos;

ChargerInfo& Base::getInfo(InnerPort port) {
    return outer->infos[port.get()];
}

rtthread::Lock Base::getLock() {
    return rtthread::Lock(outer->mutex);
}

rtthread::Mutex& Base::getMutex() {
    return outer->mutex;
}

DevConfig Base::getConfig() {
    return outer->readConfig();
}