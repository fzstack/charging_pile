#ifndef APPLICATIONS2_STATE_STORE_BASE_HXX_
#define APPLICATIONS2_STATE_STORE_BASE_HXX_

#include <memory>
#include <utilities/observer.hxx>
#include <utilities/observable.hxx>
#include <Mutex.h>
#include <config/app.hxx>

class StateStoreBase : public std::enable_shared_from_this<StateStoreBase> {
public:
    StateStoreBase();
protected:
    Observable<std::optional<State::Value>> state;
public:
    Observer<std::optional<State::Value>> oState;
};


#endif /* APPLICATIONS2_STATE_STORE_BASE_HXX_ */
