#include "air_component_base.hxx"
#include <utilities/err.hxx>

using namespace std;

AirComponentBase::AirComponentBase(std::shared_ptr<Air724> owner): owner(owner) {

}

std::vector<at_urc> AirComponentBase::onUrcTableInit() {
    throw not_implemented{};
}

at_client_t AirComponentBase::getAtClient() {
    return owner->atClient;
}

void AirComponentBase::initState(std::shared_ptr<State> state) {
    try {
        state->urcTable = std::move(onUrcTableInit());
        at_obj_set_urc_table(owner->atClient, &state->urcTable[0], state->urcTable.size());
    } catch(const not_implemented& e) {

    }
}

std::shared_ptr<at_response> AirComponentBase::createResp() {
    return std::shared_ptr<at_response>(at_create_resp(owner->kDefaultAtRespBuffSize, 0, owner->kTimeoutMs), [](auto p) {
        at_delete_resp(p);
    });
}
