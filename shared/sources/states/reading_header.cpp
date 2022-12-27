#include "../../headers/protocol_states.hpp"
#include "../../headers/messages.hpp"

#include <iostream>

using namespace uahruart::protocol::states;

ProtocolState* ReadingHeader::on_enter() {
    // Check if data is available
    if (m_state_machine->ctx.ammount_read)
        return signal(signals::BYTES_RCV);
        
    return this;
}

void ReadingHeader::on_exit() {

}


ProtocolState* ReadingHeader::signal(const Signal_t signal) {
    switch (signal) {
        case signals::BYTES_RCV: {
            // Check ammount read
            if (m_state_machine->ctx.ammount_read >= messages::Header::static_size()) {
                // Read header
                messages::Header header;
                *m_state_machine->ctx.buffer >> header;
                m_state_machine->ctx.ammount_read -= messages::Header::static_size();
                m_state_machine->ctx.message_type = header.type;

                // Update context
                // TODO: check if outside bounds
                m_state_machine->ctx.body_size = header.length;
                if (m_state_machine->ctx.message_type >= static_cast<uint16_t>(id::IDs::SIZE)) {
                    // Invalid message TODO: add redirection to flushing
                }

                m_state_machine->ctx.tagged_buffer_position = m_state_machine->ctx.buffer->beginning_rx() + messages::Header::static_size();
                
                return &m_state_machine->ctx.s_reading_body;
            }
            break;
        }
    }
    return this;
}
