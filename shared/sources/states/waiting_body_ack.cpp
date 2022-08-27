#include "../../headers/states.hpp"

using namespace protocol::states;

void WaitingBodyACK::on_enter() {
    m_state_machine->reset_read();
}

void WaitingBodyACK::on_exit() {

}

State* WaitingBodyACK::parse_byte(const char t_byte) {
    m_state_machine->push_to_buffer(t_byte);

    // Read ACK
    if (m_state_machine->m_ammount_read == (primitives::ACK::static_size() + primitives::Bool::static_size())) {
        primitives::ACK ack;
        primitives::Bool last;

        m_state_machine->m_msg_buffer.data() >> ack >> last;

        switch (ack) {
            case messages::BODY_ACK: // Body received correctly (go back to reading header)
                return &m_state_machine->s_reading_header;
                break;
            case messages::BODY_NACK: // Body invalid (send again)
                m_state_machine->send_body_now();
                on_enter();
                break;
            default: // Corrupt ACK (ask again for it)
                m_state_machine->send_ack(messages::CORR_ACK, true);
                on_enter();
                break;
        }
    }

    return this;
}
