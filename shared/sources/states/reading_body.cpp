#include "../../headers/protocol_states.hpp"

using namespace protocol::states;

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
        // FIXME: delete
        std::cout << "Ammount read " << m_state_machine->ctx.ammount_read << '\n';
        case signals::BYTES_RCV: {
            // Read header until ammount completely read
            if (m_state_machine->ctx.ammount_read >= (m_state_machine->ctx.body_size + primitives::Checksum::static_size())) {
                m_state_machine->ctx.ammount_read -= m_state_machine->ctx.body_size + primitives::Checksum::static_size();
                // Calculate checksum
                uahruart::utils::hash_t calcuated_checksum = uahruart::utils::calculate_hash(m_state_machine->ctx.tagged_buffer_position, m_state_machine->ctx.body_size);
                // Check current checksum
                primitives::Checksum checksum_in_message;
                (m_state_machine->ctx.tagged_buffer_position + m_state_machine->ctx.body_size) >> checksum_in_message;
                // Compare both checksums
                // FIXME: delete
                printf("Checksum calculated: %lu, real: %lu\n", calcuated_checksum, static_cast<size_t>(checksum_in_message));

            }
            break;
        }
    }
    return this;
}
