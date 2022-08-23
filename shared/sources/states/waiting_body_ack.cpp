#include "../../headers/states.hpp"

using namespace protocol::states;

void WaitingBodyACK::on_enter() {

}

void WaitingBodyACK::on_exit() {

}

State* WaitingBodyACK::parse_byte(const char t_byte) {

    return this;
}
