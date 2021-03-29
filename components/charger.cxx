#include "charger.hxx"

using namespace std;

Charger::Charger(
    std::shared_ptr<Relay> relay,
    std::shared_ptr<Multimeter::Channel> multimeterChannel,
    std::shared_ptr<VirtualLoadDetector> vlodet,
    std::shared_ptr<StateStore> stateStore
): relay(relay), multimeterChannel(multimeterChannel), vlodet(vlodet), stateStore(stateStore) {
    inited.onChanged += [this](auto value) {
        if(!value) return;
        this->stateStore->watch(this->relay);
        this->stateStore->watch(this->multimeterChannel);
        this->stateStore->watch(this->vlodet);
    };
};

void Charger::init() {
    inited = true;
}

void Charger::start() {
    //assertState(State::LoadInserted, "start charging");
    relay->value = Relay::On;
}

void Charger::stop() {
    //assertState(State::Charging, "stop charging");
    relay->value = Relay::Off;
}

void Charger::assertState(State::Value expectedState, string action) {
    auto state = *stateStore->oState;
        if(state != expectedState)
            throw charger_error{"failed to " + action + ", reason: "s + (state ? getStateStr(*state): "null_state")};
}
