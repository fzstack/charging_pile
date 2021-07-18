#pragma once
#include "loader.hxx"
#include <handshake.hxx>

class LowerLoader: public Loader {
public:
    virtual std::shared_ptr<App> createApp() override;
private:
    enum class Method {
        Major,
        Ota,
        Unknown,
    };
private:
    static Method handshakeTypeToMethod(HandshakeType type);

};