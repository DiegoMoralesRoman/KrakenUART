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
        case signals::BYTES_RCV: {
            // Go to reading header
            return &m_state_machine->ctx.s_reading_header;
        }
        case signals::DATA_SENT: {
            return &m_state_machine->ctx.s_waiting_ack;
        }
    }
    return this;
}
