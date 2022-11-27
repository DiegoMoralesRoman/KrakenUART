#include <iostream>

#include "shared/headers/protocol.hpp"

class Buff : public protocol::serial::Stream {
    public:
    virtual size_t read(char* buffer, const size_t ammount) override {
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
        return ammount;
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
    Buff b;
    ProtocolSM sm;
    TestState test_s(&sm);
    sm.set_state(&test_s);
    sm.signal(0);
}
