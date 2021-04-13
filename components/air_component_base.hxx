#ifndef COMPONENTS_AIR_COMPONENT_BASE_HXX_
#define COMPONENTS_AIR_COMPONENT_BASE_HXX_

#include <memory>
#include <at.h>
#include <vector>
#include <functional>
#include <utilities/observable.hxx>
#include <devices/air724.hxx>

class Air724;

class AirComponentBase: public std::enable_shared_from_this<AirComponentBase> {
protected:
    AirComponentBase(std::shared_ptr<Air724> owner);

    class State {
    public:
        std::vector<at_urc> urcTable = {};
        std::weak_ptr<AirComponentBase> inst;
    };

    virtual void init() = 0;
    virtual std::vector<at_urc> onUrcTableInit();

    at_client_t getAtClient();
    std::shared_ptr<at_response> createResp(int timeout = kTimeout);
    void initState(std::shared_ptr<State> state);

    std::shared_ptr<Air724> owner; //持有owner的引用
    static constexpr int kTimeout = 1000;
};



#endif /* COMPONENTS_AIR_COMPONENT_BASE_HXX_ */
