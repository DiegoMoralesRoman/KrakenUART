#include <iostream>

#include "shared/headers/protocol.hpp"

int main() {
    protocol::primitives::Int32 test_val = 12345;
    ProtocolSM sm;
    sm.set_state(&sm.ctx.s_idle);
}
