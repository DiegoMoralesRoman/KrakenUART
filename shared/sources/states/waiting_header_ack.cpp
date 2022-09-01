#include "../../headers/states.hpp"

using namespace protocol::states;


void WaitingHeaderACK::on_enter() {
    m_state_machine->reset_read();
}

void WaitingHeaderACK::on_exit() {

}

State* WaitingHeaderACK::parse_byte(const char t_byte) {
    m_state_machine->push_to_buffer(t_byte);

    // ==================================================
    // Read ACK
    // ==================================================
    if (m_state_machine->m_ammount_read == (primitives::ACK::static_size() + primitives::Bool::static_size())) {
        primitives::ACK ack;
        primitives::Bool last;

        m_state_machine->m_msg_buffer.data() >> ack >> last;

        switch (ack) {
            case messages::HEADER_ACK: { // Header received correctly
                m_state_machine->send_body_now();
                return &m_state_machine->s_waiting_body_ack;
                break;
            } case messages::HEADER_NACK: { // Header corrupted (send again)
                m_state_machine->send_header_now();
                on_enter();
                break;
            }
            default: // Corrupt (ask again for ack)
                // Send corrupt ack
                m_state_machine->send_ack(messages::CORR_ACK, true);
                on_enter();
                break;
        }
    }

    return this;
}
