#include "../../headers/states.hpp"

using namespace protocol::states;

void ReadingBody::on_enter() {

}

void ReadingBody::on_exit() {

}

State* ReadingBody::parse_byte(const char t_byte) {

    return this;
}

