#include "../../headers/protocol_states.hpp"

using namespace protocol::states;

ProtocolState* ReadingBody::on_enter() {
    return this;
}

void ReadingBody::on_exit() {

}

ProtocolState* ReadingBody::signal(const Signal_t signal) {
    return this;
}
