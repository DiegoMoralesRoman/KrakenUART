#include "../../headers/states.hpp"

using namespace protocol::states;

// -- DEBUG --
#include <iostream>

void ReadingBody::on_enter() {
    ack_read_pending = true;
    m_state_machine->reset_read();
}

void ReadingBody::on_exit() {

}

State* ReadingBody::parse_byte(const char t_byte) {
    m_state_machine->push_to_buffer(t_byte);

    // ==================================================
    // Read ACK
    // ==================================================
    if (ack_read_pending && (m_state_machine->m_ammount_read == (primitives::ACK::static_size() + primitives::Bool::static_size()))) {
        ack_read_pending = false;

        primitives::ACK ack;
        primitives::Bool last;
        m_state_machine->m_msg_buffer.data() >> ack >> last;

        // Reset reading header
        m_state_machine->reset_read();

        // Check ACK
        switch (ack) {
            case messages::ACKTypes::UNDEF_ACK: // First time sending body
                break;
            default: // Corrupt header
                // Send HEADER ACK only if there is no more data left
                if (sent_nack) {
                    m_state_machine->send_ack(messages::BODY_NACK, true);
                    on_enter();
                } else if (last) {
                    m_state_machine->send_ack(messages::HEADER_ACK, true);
                }
                break;
        }

        sent_nack = false;
        return this;
    }

    // ==================================================
    // Read body
    // ==================================================
    if (!ack_read_pending && (m_state_machine->m_ammount_read == (m_body_len + primitives::Checksum::static_size()))) {
        // Read body
        primitives::Checksum checksum;
        (m_state_machine->m_msg_buffer.data() + m_body_len) >> checksum;

        // Validate checksum
        primitives::Checksum::Underlying_t calculated_checksum = uahruart::utils::calculate_hash(m_state_machine->m_msg_buffer.data(), m_body_len);

        if (static_cast<primitives::Checksum::Underlying_t>(checksum) == calculated_checksum) { // Valid checksum send body ACK
            m_state_machine->send_ack(messages::BODY_ACK, true);
            return &m_state_machine->s_reading_header;
        } else { // Invalid checksum send body NACK
            sent_nack = true;
            m_state_machine->send_ack(messages::BODY_NACK, true);
            on_enter();
        }
    }

    return this;
}

void ReadingBody::init(size_t len, decltype(messages::Header::message_type)::Underlying_t type) {
    m_body_len = len;
    m_message_type = type;
}

