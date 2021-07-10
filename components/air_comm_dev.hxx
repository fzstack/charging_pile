#pragma once

#include <devices/air724.hxx>
#include "comm_dev.hxx"
#include <memory>

class AirCommDev: public CommDev {
public:
    AirCommDev(std::shared_ptr<Air724> air724);
    virtual void reinit() override;
private:
    std::shared_ptr<Air724> air724;
};
