#ifndef STATES_HPP
#include "../headers/states.hpp"
#endif 

#include <utility>

using namespace uahruart::protocol::states;

template<typename Context>
StateMachine<Context>::~StateMachine() {
    if (m_current_state != nullptr)
        m_current_state->on_exit();
}

// TODO: update state machine on on_enter method
template<typename Context>
void StateMachine<Context>::set_state(State<Context>* state) {
    m_current_state = state;
    if (m_prev_state != nullptr)
        m_prev_state->on_exit();
    m_current_state->on_enter();
    m_prev_state = m_current_state;
}

template<typename Context>
void StateMachine<Context>::signal(const Signal_t signal) {
    m_current_state = m_current_state->signal(signal);
    // Update state while changes are being made to the state machine
    while (m_current_state != m_prev_state) {
        auto* tmp_state = m_prev_state;
        m_prev_state = m_current_state;

        m_current_state = m_current_state->on_enter();
        tmp_state->on_exit();
    }
}

