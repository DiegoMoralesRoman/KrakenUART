#include "../../headers/states.hpp"

using namespace protocol::states;

void IgnoringN::on_enter() {

}

void IgnoringN::on_exit() {

}

State* IgnoringN::parse_byte(const char t_byte) {
    if (--to_ignore == 0) {
        return return_state;
    }

    return this;
}

void IgnoringN::ignore(size_t ammount, State *return_state) {
    to_ignore = ammount;
    this->return_state = return_state;
}
