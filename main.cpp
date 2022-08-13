#include <iostream>

#include "shared/primitives.hpp"
#include "shared/messages.hpp"

int main() {
    char buffer[100] = {0,};

    protocol::messages::Header header;
    header.len = 12345;
    header.message_checksum = 123456789;
    header.header_checksum = 87654321;

    buffer << header;

    for (int i = 0; i < sizeof(buffer); i++)
        std::cout << std::hex << static_cast<int>((unsigned char)buffer[i]) << ' ';
    
    protocol::messages::Header h2;
    buffer >> h2;

    std::cout << std::dec;
    std::cout << '\n' << (uint32_t)h2.len << ' ' << (uint32_t)h2.message_checksum << ' ' << (uint32_t)h2.header_checksum << '\n';

    return 0;
}
