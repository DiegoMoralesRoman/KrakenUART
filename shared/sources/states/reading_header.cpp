#include "../../headers/protocol_states.hpp"

using namespace protocol::states;

void ReadingHeader::on_enter() {

}

void ReadingHeader::on_exit() {

}

ProtocolState* ReadingHeader::signal(const Signal_t signal) {

    return this;
}
