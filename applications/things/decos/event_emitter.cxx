#include <config/app.hxx>
#include "event_emitter.hxx"
#include <utilities/mp.hxx>

using namespace std;
using namespace Things::Decos;

EventEmitter::EventEmitter(outer_t* outer): Base(outer) {
    inited.onChanged += [this](auto value) {
        if(!value) return;
        for(rt_uint8_t i = 0u; i < Config::Bsp::kPortNum; i++) {
            auto guard = getLock();
            getInfo(InnerPort{i}).charger->stateStore->oState += [this, i](auto value) {
              if(!value) return;
              switch(*value) {
              case State::LoadInserted:
                  this->outer->onPortAccess(InnerPort{i});
                  break;
              case State::LoadNotInsert:
                  this->outer->onPortUnplug(InnerPort{i});
                  break;
              default: break;
              }
            };
        }
    };
}

void EventEmitter::init() {
    inited = true;
}

