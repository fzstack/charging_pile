#include <config/app.hxx>
#include "event_emitter.hxx"
#include <utilities/mp.hxx>

using namespace std;
using namespace Things::Decos;

void EventEmitter::onStateChanged(InnerPort port, State::Value state) {
    auto guard = getLock();
    switch(state) {
    case State::LoadInserted:
      this->outer->onPortAccess(port);
      break;
    case State::LoadNotInsert:
      this->outer->onPortUnplug(port);
      break;
    default: break;
    }
}

