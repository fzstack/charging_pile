#include "last_charger.hxx"

using namespace std;

LastCharger::LastCharger(): oWaitingToStart(waitingToStart) {

}

void LastCharger::watch(shared_ptr<StateStoreBase> store) {
    store->oState += [=](auto value) {
        if(!value) return;
        if(*value == State::LoadInserted) {
            waitingToStart = store;
        } else if(*waitingToStart && **waitingToStart == store && *value != State::LoadInserted) {
            waitingToStart = nullopt;
        }
    };
}

