#include "../../headers/states.hpp"

using namespace protocol::states;

void WaitingForResponse::on_enter() {

}

void WaitingForResponse::on_exit() {

}

State* WaitingForResponse::parse_byte(const char t_byte) {
    
    return this;
}
