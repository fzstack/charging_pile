#include "base.hxx"

using namespace rtthread;
using namespace Things::Decos;

ChargerInfo& Base::getInfo(InnerPort port) {
    return outer->infos[port.get()];
}

rtthread::Lock Base::getLock() {
    return Lock(outer->mutex);
}
