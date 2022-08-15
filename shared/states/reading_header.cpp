#include "../states.hpp"

using namespace protocol::states;

void ReadingHeader::on_enter() {

}

void ReadingHeader::on_exit() {

}

State* ReadingHeader::parse_byte(const char t_byte) {
    
    return this;
}
