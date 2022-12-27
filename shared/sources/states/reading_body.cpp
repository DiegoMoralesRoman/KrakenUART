#include "../../headers/protocol_states.hpp"

using namespace uahruart::protocol::states;

// FIXME: delete
#include <iostream>


ProtocolState* ReadingBody::on_enter() {
    if (m_state_machine->ctx.ammount_read)
        return signal(signals::BYTES_RCV);
        
    return this;
}

void ReadingBody::on_exit() {

}

ProtocolState* ReadingBody::signal(const Signal_t signal) {
    switch (signal) {
        case signals::BYTES_RCV: {
            // Read header until ammount completely read
            if (m_state_machine->ctx.ammount_read >= (m_state_machine->ctx.body_size + primitives::Checksum::static_size())) {
                m_state_machine->ctx.ammount_read -= m_state_machine->ctx.body_size + primitives::Checksum::static_size();
                // Calculate checksum
                uahruart::utils::hash_t calcuated_checksum = uahruart::utils::calculate_hash(m_state_machine->ctx.buffer->beginning_rx(), m_state_machine->ctx.body_size + messages::Header::static_size());
                // Check current checksum
                primitives::Checksum checksum_in_message;
                (m_state_machine->ctx.tagged_buffer_position + m_state_machine->ctx.body_size) >> checksum_in_message;
                // Compare both checksums
                if (calcuated_checksum != static_cast<decltype(checksum_in_message)::Underlying_t>(checksum_in_message)) {
                    // Invalid checksum
                    // FIXME: delete
                    std::cout << "\033[31mInvalid message\033[0m\n";
                } else { // Checksum correct 
                    // FIXME: delete
                    std::cout << "\033[33mValid message\033[0m\n";
                    std::cout << static_cast<int>(m_state_machine->ctx.tagged_buffer_position[0]) << '\n';
                    // Invoke callback
                    m_state_machine->ctx.callback_store.invoke(m_state_machine->ctx.message_type, m_state_machine->ctx.tagged_buffer_position);
                }
            }
            break;
        }
    }
    return this;
}
