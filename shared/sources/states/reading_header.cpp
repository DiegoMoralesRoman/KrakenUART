#include "../../headers/protocol_states.hpp"

using namespace protocol::states;

ProtocolState* ReadingHeader::on_enter() {
    
    return this;
}

void ReadingHeader::on_exit() {

}

ProtocolState* ReadingHeader::signal(const Signal_t signal) {

    return this;
}
