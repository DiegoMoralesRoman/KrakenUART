#include "../headers/states.hpp"

using namespace protocol::states;

// State machine
void StateMachine::reset() {
    m_current_state = INIT_STATE;
    m_prev_state = INIT_STATE;
    m_current_state->on_enter();
}

void StateMachine::parse_byte(const char t_byte) {
    // Store state before parsing
    State* prev_state = m_current_state;
    // Parse byte in the current state
    m_current_state = m_current_state->parse_byte(t_byte);
    
    // If the state has changed call on_enter and on_exit methods
    if (prev_state != m_current_state) {
        prev_state->on_exit();
        m_current_state->on_enter();

        m_prev_state = prev_state;
    }
}

void StateMachine::send_message(const primitives::Serializable &&serializable) {
    set_current_message(std::move(serializable));
    send_header_now();
}

void StateMachine::send_message(const messages::ProtocolMessage&& message) {
    set_current_message(std::move(message));
    send_header_now();
}

// Utility methods
void StateMachine::send_header_now() {
    m_current_state = &s_waiting_header_ack;
    
    // Send header
    send_ack(messages::UNDEF_ACK, false);
    m_send_buffer(m_write_start, messages::Header::static_size());
}

void StateMachine::send_body_now() {
    // Send body and wait for body ack
    char* body_start = m_write_start + messages::Header::static_size();
    size_t msg_size = &m_msg_buffer.back() - body_start;
    send_ack(messages::UNDEF_ACK, false);

    // Get body checksum and send
    primitives::Checksum chk = uahruart::utils::calculate_hash(body_start, msg_size);
    char buff[chk.size()];
    buff << chk;

    m_send_buffer(body_start, msg_size);
    // Send checksum
    m_send_buffer(buff, chk.size());
}

void StateMachine::push_to_buffer(uint8_t t_byte) {
    if (m_ammount_read < PROTOCOL_BUFFER_SIZE)
        m_msg_buffer[m_ammount_read++] = t_byte;
}

void StateMachine::reset_read() {
    m_ammount_read = 0;
}

void StateMachine::reset_write() {
    m_write_start = m_msg_buffer.data() + m_msg_buffer.size();
}

// -- DEBUG --
#include <iostream>

// Setting the buffer
void StateMachine::set_current_message(const primitives::Serializable&& msg) {
    m_write_start = m_msg_buffer.data() + (m_msg_buffer.size() - msg.size());
    m_msg_buffer.data() << msg;
}


void StateMachine::set_current_message(const messages::ProtocolMessage&& msg) {
    reset_write();
    m_write_start = &m_msg_buffer.back() - (messages::Header::static_size() + msg.size());

    // Prepare header
    messages::Header header;
    header.message_type = msg.type();
    header.len = msg.size();

    // Serialize everything
    m_write_start << header << msg;
}

void StateMachine::send_ack(std::uint32_t ack, bool last) {
    char buff[primitives::ACK::static_size() + primitives::Bool::static_size()];
    buff << primitives::ACK(ack) << primitives::Bool(last);
    // Send buffer
    m_send_buffer(buff, sizeof(buff));
}

