#include "air_comm_dev.hxx"

AirCommDev::AirCommDev(std::shared_ptr<Air724> air724): air724(air724) {

}

void AirCommDev::reinit() {
    air724->reinit();
}
