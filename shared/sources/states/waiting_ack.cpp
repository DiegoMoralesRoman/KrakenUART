#include "../../headers/protocol_states.hpp"
#include "../../headers/messages.hpp"

using namespace protocol::states;

ProtocolState* WaitingACK::on_enter() {
    return this;
}

void WaitingACK::on_exit() {

}

ProtocolState* WaitingACK::signal(const Signal_t signal) {
    switch (signal) {
        case signals::BYTES_RCV: {
            break;
        }
    }
    return this;
}
