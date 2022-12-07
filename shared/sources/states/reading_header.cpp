#include "../../headers/protocol_states.hpp"

#include "../../headers/messages.hpp"

using namespace protocol::states;

ProtocolState* ReadingHeader::on_enter() {
    bytes_read = 0;
    return this;
}

void ReadingHeader::on_exit() {

}

ProtocolState* ReadingHeader::signal(const Signal_t signal) {
    switch (signal) {
        case signals::BYTES_RCV: {
            ++bytes_read;
            
            // Check if header is already read
            if (bytes_read == sizeof(messages::Header))
                return &m_state_machine->ctx.s_reading_body;
            break;
        }
    }
    return this;
}
