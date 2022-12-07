#include <stdio.h>
#include "shared/headers/protocol.hpp"


ProtocolSM sm1;
void sm1_rcv_bytes(const char* buffer, size_t ammount) {
    sm1.ctx.current_buffer = const_cast<char*>(buffer);
    sm1.ctx.ammount_read = ammount;
    sm1.signal(signals::BYTES_RCV);
}

class SerialBuffer : virtual public ProtocolBuffer {
    public:
        bool check_hash(uahruart::utils::hash_t hash) override {
            return true;
        }


        char* operator<<(const protocol::serial::Serializable& serializable) override {
            printf("Writing\n");
            ammount_written = serializable.size();
            return tx_buffer << serializable;
        }


        const char* operator>>(protocol::serial::Serializable& serializable) const override {
            printf("Reading\n");
            ammount_read = serializable.size();
            return rx_buffer >> serializable;
        }

        void print_tx() {
            for (size_t i = 0; i < sizeof(tx_buffer); i++)
                printf("%i ", static_cast<int>(tx_buffer[i]));
        }

        void print_rx() {
            for (size_t i = 0; i < sizeof(rx_buffer); i++)
                printf("%i ", static_cast<int>(rx_buffer[i]));
        }

    public: // Theoretically private
        mutable size_t ammount_read = 0;
        size_t ammount_written = 0;
        char tx_buffer[64];
        mutable char rx_buffer[64];
} sm1_buffer;


int main() {
    protocol::primitives::Int32 test_val = 12345;
    sm1.set_state(&sm1.ctx.s_idle);
    sm1.ctx.buffer = &sm1_buffer;
    sm1 << test_val;
    printf("Written: %lu\n", sm1_buffer.ammount_written);
    sm1_buffer.print_tx();
}
