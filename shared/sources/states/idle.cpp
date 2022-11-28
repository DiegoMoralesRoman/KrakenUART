#include "../../headers/protocol_states.hpp"

using namespace protocol::states;

void Idle::on_enter() {

}

void Idle::on_exit() {

}

ProtocolState* Idle::signal(const Signal_t signal) {
    switch (signal) {
        case signals::BYTE_RCV:
            return &m_state_machine->ctx.s_idle;
    }

    return this;
}
