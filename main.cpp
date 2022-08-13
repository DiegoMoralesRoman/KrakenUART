#include <iostream>

#include "shared/primitives.hpp"

int main() {
    char buffer[100] = {0,};
    protocol::primitives::Int32 n = 10;
    protocol::primitives::Int16 n2 = 1234;
    n = 14;
    buffer << n << n << n2 << n2 << protocol::primitives::Int8(123);
    for (int i = 0; i < sizeof(buffer); i++)
        std::cout << std::hex << static_cast<int>((unsigned char)buffer[i]) << ' ';

    protocol::primitives::Int32 recn1, recn2;
    protocol::primitives::Int16 recn3, recn4;
    protocol::primitives::Int8 recn5;

    buffer >> recn1 >> recn2 >> recn3 >> recn4 >> recn5;
    std::cout << '\n' << std::dec << recn1 << ' ' << recn2 << ' ' << recn3 << ' ' << recn4 << ' ' << (uint32_t)recn5 << '\n';
    return 0;
}
