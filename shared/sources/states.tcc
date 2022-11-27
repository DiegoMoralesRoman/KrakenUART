#ifndef STATES_HPP
#include "../headers/states.hpp"
#endif 

#include <utility>

using namespace protocol::states;

template<typename Context>
StateMachine<Context>::~StateMachine() {
    if (m_current_state != nullptr)
        m_current_state->on_exit();
}

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
    if (m_current_state != m_prev_state) {
        m_current_state->on_enter();
        m_prev_state->on_exit();
    }
    m_prev_state = m_current_state;
}

