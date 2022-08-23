#include "../headers/states.hpp"

using namespace protocol::states;

// State machine
void StateMachine::reset() {
    m_current_state = INIT_STATE;
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
    }
}

void StateMachine::send_message(const primitives::Serializable &&serializable) {
    set_current_message(std::move(serializable));
}

void StateMachine::send_message(const messages::ProtocolMessage&& message) {
    set_current_message(std::move(message));
}

// Utility methods
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

// Setting the buffer
void StateMachine::set_current_message(const primitives::Serializable&& msg) {
    m_write_start = m_msg_buffer.data() + (m_msg_buffer.size() - msg.size());
    m_msg_buffer.data() << msg;
}


void StateMachine::set_current_message(const messages::ProtocolMessage&& msg) {
    reset_write();
    m_write_start = m_msg_buffer.data() + (m_msg_buffer.size() - (messages::Header::static_size() + msg.size()));

    // Prepare header
    messages::Header header;
    header.message_type = msg.type();
    header.len = msg.size();

    // Serialize everything
    m_write_start << header << msg;
}

