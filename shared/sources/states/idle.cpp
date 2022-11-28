#include "../../headers/protocol_states.hpp"

using namespace protocol::states;

ProtocolState* Idle::on_enter() {

    return this;
}

void Idle::on_exit() {

}

// -- DEBUG --
#include <iostream>

ProtocolState* Idle::signal(const Signal_t signal) {
    switch (signal) {
        case signals::BYTE_RCV:
            std::cout << static_cast<int>(m_state_machine->ctx.last_rcv_byte) << ' ';
    }

    return this;
}
