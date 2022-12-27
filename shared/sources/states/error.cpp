#include "../../headers/protocol_states.hpp"
#include "../../headers/messages.hpp"

using namespace uahruart::protocol::states;

ProtocolState* Error::on_enter() {
    return this;
}

void Error::on_exit() {

}

ProtocolState* Error::signal(const Signal_t signal) {
    switch (signal) {
        case signals::BYTES_RCV: {
            break;
        }
    }
    return this;
}
