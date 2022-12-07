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

    return this;
}
