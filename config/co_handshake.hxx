#pragma once
#include <cstdint>

enum class HandshakeType: uint8_t {
    Major,
    Ota,
};

namespace Packets {
struct Handshake {
    bool sync;
    HandshakeType type;
};
}