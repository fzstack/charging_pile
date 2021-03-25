#ifndef COMPONENTS_STATE_STORE_HXX_
#define COMPONENTS_STATE_STORE_HXX_

#include "state_store_base.hxx"
#include <devices/relay.hxx>
#include "multimeter.hxx"
#include "virtual_load_detector.hxx"
#include "timer.hxx"

class StateStore: public StateStoreBase {
    using StateStoreBase::StateStoreBase;
public:
    StateStore();
    void watch(std::shared_ptr<Relay> relay);
    void watch(std::shared_ptr<Multimeter::Channel> multimeterChannel);
    void watch(std::shared_ptr<VirtualLoadDetector> virtualLoadDetector);
private:
    void update();

private:
    std::shared_ptr<Relay> relay;
    std::shared_ptr<Multimeter::Channel> multimeterChannel;
    std::shared_ptr<VirtualLoadDetector> virtualLoadDetector;

private:
    static const char* kMutex;
    static rtthread::Mutex mutex;

};

#endif /* COMPONENTS_STATE_STORE_HXX_ */
