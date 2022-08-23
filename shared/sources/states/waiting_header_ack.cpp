#include "../../headers/states.hpp"

using namespace protocol::states;

void WaitingHeaderACK::on_enter() {

}

void WaitingHeaderACK::on_exit() {

}

State* WaitingHeaderACK::parse_byte(const char t_byte) {
    
    return this;
}
