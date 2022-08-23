#include "../../headers/states.hpp"
#include "../../headers/messages.hpp"

// -- DEBUG --
#include <iostream>
// -- DEBUG --

using namespace protocol::states;

void ReadingHeader::on_enter() {
    m_state_machine->reset_read();
    m_state_machine->reset_write();
}

void ReadingHeader::on_exit() {
}

State* ReadingHeader::parse_byte(const char t_byte) {
    return this;
}
