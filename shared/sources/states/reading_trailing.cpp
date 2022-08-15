#include "../../headers/states.hpp"

using namespace protocol::states;

void ReadingTrailing::on_enter() {

}

void ReadingTrailing::on_exit() {

}

State* ReadingTrailing::parse_byte(const char t_byte) {
    
    return this;
}
