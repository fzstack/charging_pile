#include "state_store.hxx"
#include <Lock.h>

using namespace rtthread;

StateStore::StateStore() {

}

void StateStore::watch(std::shared_ptr<Relay> relay) {
    this->relay = relay;
    relay->value.onChanged += [this](auto value){
        if(value == Relay::Off) {
            state = State::LoadWaitRemove; //TODO: 在一秒后调用update //LoadWaitRemove
            return;
        }
        update();
    };
}

void StateStore::watch(std::shared_ptr<Multimeter::Channel> multimeterChannel) {
    this->multimeterChannel = multimeterChannel;
    multimeterChannel->current += [this](auto){update();};
    multimeterChannel->voltage += [this](auto){update();};
}

void StateStore::watch(std::shared_ptr<VirtualLoadDetector> virtualLoadDetector) {
    this->virtualLoadDetector = virtualLoadDetector;
    virtualLoadDetector->oState += [this](auto value){
        if(value && *value) {
            state = State::LoadInserted;
            return;
        }
        update();
    };
}

void StateStore::update() {
    auto lock = Lock(mutex);

    if(relay) {
        //如果继电器闭合，那么必在充电
        if(*relay->value == Relay::On) {
            state = State::Charging;
            return;
        }
    }

    if(virtualLoadDetector) {
        //如果负载未接入, 那么必是未接入状态
        auto lodet = virtualLoadDetector->oState.value();
        if(lodet && !*lodet) {
            state = State::LoadNotInsert;
            return;
        }
    }

    //如果继电器断开、负载已接入，那么可能有两种情况
    //负载已接入状态、充电已完成状态
}

Mutex StateStore::mutex = {kMutex};
