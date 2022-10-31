#include <iostream>

#include "shared/headers/protocol.hpp"

class Buff : public protocol::serial::Stream {
    public:
    virtual void read(char* buffer, const size_t ammount) override {
        if (prev_writing) {
            std::cout << "Reseting buffer\n";
            position = 0;
        }
        prev_writing = false;

        std::cout << "Reading ";
        for (size_t i = 0; i < ammount; i++) {
            std::cout << static_cast<int>(m_buffer[position]) << ' ';
            buffer[i] = m_buffer[position++];
        }
        std::cout << '\n';
    }

    virtual void write(const char* buffer, const size_t len) override {
        prev_writing = true;
        std::cout << "Writing ";
        for (size_t i = 0; i < len; i++) {
            m_buffer[position++] = buffer[i];
            std::cout << static_cast<int>(m_buffer[position - 1]) << ' ';
        }
        std::cout << '\n';
    }
    uint8_t m_buffer[128] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    size_t position = 0;
    bool prev_writing = true;

    virtual void flush() override {
        std::cout << "Flushing buffer\n";
    }
};

int main() {
    protocol::serial::Base128Stream base128stream;
    Buff b;
    base128stream.set_connection_stream(&b);

    protocol::primitives::Int32 n1 = 123456789;
    protocol::primitives::Int32 n2 = 987654321;
    base128stream << n1 << n2;
    base128stream.flush();

    protocol::primitives::Int32 other_n1, other_n2;
    base128stream >> other_n1 >> other_n2;
    std::cout << "Output: " << other_n1 << ", " << other_n2 << '\n';
}
