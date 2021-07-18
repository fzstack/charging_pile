#pragma once
#include "app.hxx"
#include <components/handshake.hxx>

class LowerApp: public App {
public:
    virtual std::shared_ptr<Handshake> getHandshake() = 0;
};