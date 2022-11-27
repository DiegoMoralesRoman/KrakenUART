#include "../../headers/protocol_states.hpp"

using namespace protocol::states;

#include <iostream>

void TestState::on_enter() {
    std::cout << "Hola mundo\n";
}

void TestState::on_exit() {
    std::cout << "Saliendo del estado\n";
}

ProtocolState* TestState::signal(const Signal_t signal) {
    std::cout << "Signaled " << signal << '\n';
    return this;
}

