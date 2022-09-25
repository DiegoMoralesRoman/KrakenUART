#include "../../headers/states.hpp"

// -- DEBUG --
#include <iostream>
// -----------

using namespace protocol::states;


void WaitingHeaderACK::on_enter() {
    m_state_machine->reset_read();
    m_state_machine->m_has_send_priority = false;
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
                m_state_machine->comm_started = false;
                return &m_state_machine->s_waiting_body_ack;
                break;
            } case messages::HEADER_NACK: { // Header corrupted (send again)
                m_state_machine->send_header_now();
                m_state_machine->comm_started = false;
                on_enter();
                break;
            } case messages::FIRST_MSG: { // Collision detected
                std::cout << "FIRST_MSG recibido\n";
                // Check if has less priority to change state
                if (!m_state_machine->m_has_send_priority) {
                    std::cout << "No tiene prioridad\n";
                    m_state_machine->send_ack(messages::HEADER_NACK, true);
                    m_state_machine->comm_started = false;
                    m_state_machine->s_ignoring_n.ignore(messages::Header::static_size(), &m_state_machine->s_reading_header);
                    return &m_state_machine->s_ignoring_n;
                    //return &m_state_machine->s_reading_header;
                }
                std::cout << "Tiene prioridad\n";
                // Ignore next header
                m_state_machine->s_ignoring_n.ignore(messages::Header::static_size(), this);
                return &m_state_machine->s_ignoring_n;
                break;
            } default: // Corrupt (ask again for ack)
                // Send corrupt ack
                // Check if is first message
                if (m_state_machine->comm_started)
                    m_state_machine->send_ack(messages::FIRST_MSG, true);
                else
                    m_state_machine->send_ack(messages::CORR_ACK, true);
                on_enter();
                break;
        }
    }

    return this;
}
