#include "../headers/states.hpp"

using namespace protocol::states;

// State machine
void StateMachine::reset() {
    m_current_state = INIT_STATE;
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

