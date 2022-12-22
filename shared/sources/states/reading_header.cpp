#include "../../headers/protocol_states.hpp"
#include "../../headers/messages.hpp"

using namespace protocol::states;

// FIXME: delete
#include <iostream>

ProtocolState* ReadingHeader::on_enter() {
    // FIXME: delete
    std::cout << "On_enter reading header\nAmmount: " << m_state_machine->ctx.ammount_read << '\n';
    // Check if data is available
    if (m_state_machine->ctx.ammount_read)
        return signal(signals::BYTES_RCV);
        
    return this;
}

void ReadingHeader::on_exit() {

}


ProtocolState* ReadingHeader::signal(const Signal_t signal) {
    // FIXME: delete
    std::cout << "Reading data\n";
    switch (signal) {
        case signals::BYTES_RCV: {
            // Check ammount read
            if (m_state_machine->ctx.ammount_read >= messages::Header::static_size()) {
                // Read header
                messages::Header header;
                *m_state_machine->ctx.buffer >> header;
                m_state_machine->ctx.ammount_read -= messages::Header::static_size();

                // FIXME: delete (DEBUG)
                std::cout << "Header read!!\n";
                std::cout << "Message size: " << static_cast<size_t>(header.length) << '\n';
                // --------------------
                // Update context
                // TODO: check if outside bounds
                m_state_machine->ctx.body_size = header.length;
                m_state_machine->ctx.tagged_buffer_position = m_state_machine->ctx.buffer->beginning_rx() + messages::Header::static_size();
                
                return &m_state_machine->ctx.s_reading_body;
            }
            break;
        }
    }
    return this;
}
