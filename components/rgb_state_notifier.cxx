#include "rgb_state_notifier.hxx"
#include <utilities/lock_guard.hxx>
#include <map>
#include <string>
#include <functional>
#include <Lock.h>

#define LOG_TAG "dev.rgb"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;
using namespace rtthread;

RgbStateNotifier::RgbStateNotifier(std::shared_ptr<RgbLight> light): light(light) {
    k.onChanged += [this](auto value) {
        render();
    };

    timer.onRun += [this] {
        k++;
    };
    timer.start();
}

void RgbStateNotifier::watch(std::shared_ptr<StateStoreBase> store) {
    this->store = store;
    store->oState += [this](auto next) {
        render();
    };
}

void RgbStateNotifier::render() {
    auto guard = Lock(mutex);

    if(!store || !store->oState.value()) {
        light->r = light->g = light->b = false;
        return;
    }

    switch(*store->oState.value()) {
        case State::LoadNotInsert:
            light->g = true;
            light->r = light->b = false;
            break;
        case State::LoadInserted:
            light->r = *k % 2;
            light->g = light->b = false;
            break;
        case State::Charging:
            light->g = *k % 2;
            light->r = light->b = false;
            break;
        case State::LoadWaitRemove:
            light->b = true;
            light->r = light->g = false;
            break;
        case State::Error:
            light->g =  *k % 2;
            light->r = !(*k % 2);
            light->b = false;
            break;
    }
}

Timer RgbStateNotifier::timer = {kRoundDurMs, kTimer};
Mutex RgbStateNotifier::mutex = {kMutex};
