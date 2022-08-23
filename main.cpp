#include <iostream>

#include "shared/headers/primitives.hpp"
#include "shared/headers/messages.hpp"
#include "shared/headers/states.hpp"

#include <thread>
#include <mutex>

int main() {
    protocol::states::StateMachine sm;

    sm.m_send_buffer = [](const char* msg, size_t len) {
        std::cout << std::hex;
        for (size_t i = 0; i < len; i++)
            std::cout << static_cast<uint16_t>(msg[i]) << ' ';
        std::cout << std::dec;
    };

    protocol::messages::Admin msg;
    msg.ack = 123;

    sm.send_message(std::move(msg));

    // for (size_t i = 0; i < msg.size(); i++) {
    //     sm.parse_byte(buff[i]);
    // }

    return 0;
}
