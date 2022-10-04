#include <iostream>

#include "shared/headers/protocol.hpp"

class Buff : public protocol::base128::Stream {
    public:
    virtual void read(char* buffer, const size_t ammount) override {
        std::cout << "Reading ";
        for (size_t i = 0; i < ammount; i++) {
            std::cout << static_cast<int>(m_buffer[position]) << ' ';
            buffer[i] = m_buffer[position++];
        }
        std::cout << '\n';
    }

    virtual void write(const char* buffer, const size_t len) override {
        std::cout << "Writing ";
        for (size_t i = 0; i < len; i++) {
            m_buffer[position++] = buffer[i];
            std::cout << static_cast<int>(m_buffer[position - 1]) << ' ';
        }
        std::cout << '\n';
    }
    uint8_t m_buffer[128] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t position = 0;

    virtual void flush() override {
        std::cout << "Flushing buffer\n";
    }
};

int main() {
    protocol::primitives::Int32 value1 = 123456789;
    protocol::primitives::Int32 value2 = 87654321;
    Buff b;
    b << value1 << value2;
    b.position = 0;
    decltype(value1) other1;
    decltype(value2) other2;
    b.flush();
    b >> other1 >> other2;
    std::cout << (uint32_t)other1 << ' ' << (uint32_t)other2 << '\n';
}
