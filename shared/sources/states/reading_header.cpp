#include "../../headers/states.hpp"
#include "../../headers/messages.hpp"

using namespace protocol::states;

void ReadingHeader::on_enter() {
    ack_read_pending = true;
    m_state_machine->reset_read();
}

void ReadingHeader::on_exit() {
    sent_nack = false;
}


State* ReadingHeader::parse_byte(const char t_byte) {
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
            case messages::ACKTypes::UNDEF_ACK: // First time sending header
                break;
            default: // Corrupt header
                // Send BODY ACK only if there is no more data left
                if (sent_nack) {
                    m_state_machine->send_ack(messages::HEADER_NACK, true);
                    on_enter();
                } else if (last)
                    m_state_machine->send_ack(messages::BODY_ACK, true);
                break;
        }

        sent_nack = false;
        return this;
    }

    // ==================================================
    // Read body
    // ==================================================
    if (!ack_read_pending && (m_state_machine->m_ammount_read == messages::Header::static_size())) {
        // Deserialize header
        messages::Header header;
        m_state_machine->m_msg_buffer.data() >> header;

        char ack_buffer[primitives::ACK::static_size()];
        if (header.is_valid()) {
            // Send HEADER ACK
            m_state_machine->send_ack(messages::HEADER_ACK, true);

            // Change state to reading body
            m_state_machine->s_reading_body.init(header.len, header.message_type);
            return &m_state_machine->s_reading_body;
        } else {
            // Send HEADER NACK
            sent_nack = true;
            m_state_machine->send_ack(messages::HEADER_NACK, true);
            on_enter();
        }
    }
    return this;
}
