#include <iostream>

#include "shared/primitives.hpp"
#include "shared/messages.hpp"

int main() {
    char buffer[100] = {0,};

    protocol::messages::Admin msg;
    msg.len = 12345;

    msg.ack = 14;

    buffer << msg;

    for (int i = 0; i < sizeof(buffer); i++)
        std::cout << std::hex << static_cast<int>((unsigned char)buffer[i]) << ' ';
    
    protocol::messages::Admin admin;
    buffer >> admin;

    std::cout << std::dec;
    std::cout << '\n' << (uint32_t)admin.len << ' ' << (uint32_t)admin.header_checksum << ' ' << (int)(uint8_t)admin.message_type << '\n';
    std::cout << (int)admin.ack << ' ' << (bool)admin.is_valid() << '\n';

    return 0;
}
